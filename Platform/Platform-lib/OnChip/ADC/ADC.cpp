//
// Created by Whisky on 1/10/2024.
//

#include "ADC.hpp"
#include "Platform-Operation/Platform-Operation-STM32_DMA.h"
#include "Platform-Operation/Platform-Operation-STM32_SYS.h"
using namespace STM32::STM32_ADC;
using namespace STM32::STM32_DMA;
using namespace STM32::STM32_TIM;

ADC::ADC(uint8_t ADC_ID, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
         : adc(STM32::STM32_ADC::MapADC[ADC_ID]), isUseDma(false), isUseTimer(false)
{
    Calibrate(CalibrationMode, SingleDiff);     //开始校准
    while (LL_ADC_IsCalibrationOnGoing(adc));   //等待校准完成

    LL_ADC_SetBoostMode(adc, LL_ADC_BOOST_MODE_50MHZ);
}

ADC::ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t TIMER_ID, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
                :   adc(MapADC[ADC_ID]), adc_dma(MapDMA[DMA_ID]), adc_dma_stream(MapDMA_Stream[STREAM_ID]),
                    isUseDma(true), isUseTimer(true)
{
    ADC(ADC_ID, DMA_ID, STREAM_ID, TIMER_ID, channel1, CalibrationMode, SingleDiff);
}

ADC::ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t PWM_TIMER_ID, Timer_Channel PWM_Channel,
         STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode, STM32::STM32_ADC::ADC_SingleDiff SingleDiff)
                :   adc(MapADC[ADC_ID]), adc_dma(MapDMA[DMA_ID]), adc_dma_stream(MapDMA_Stream[STREAM_ID]),
                    isUseDma(true), isUseTimer(true)
{
    timer = PWM(PWM_TIMER_ID, PWM_Channel);

    Calibrate(CalibrationMode, SingleDiff);     //开始校准
    while (LL_ADC_IsCalibrationOnGoing(adc));   //等待校准完成

    LL_ADC_REG_SetDataTransferMode(adc, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    LL_ADC_SetBoostMode(adc, LL_ADC_BOOST_MODE_50MHZ);
}

ADC &ADC::Calibrate(ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff) {
    MODIFY_REG(adc->CR,
               ADC_CR_ADCALLIN | ADC_CR_ADCALDIF | ADC_CR_BITS_PROPERTY_RS,
               ADC_CR_ADCAL | (CalibrationMode & ADC_CALIB_MODE_MASK) | (SingleDiff & ADC_SINGLEDIFF_CALIB_START_MASK));
    return *this;
}

ADC &ADC::Start(uint32_t SampleRate) {
    LL_ADC_Enable(adc);  		                //使能ADC
    while(LL_ADC_IsActiveFlag_ADRDY(adc) != SET);
    if (isUseDma)
    {
        LL_DMA_SetPeriphAddress(adc_dma, adc_dma_stream, LL_ADC_DMA_GetRegAddr(adc, LL_ADC_DMA_REG_REGULAR_DATA));
        LL_DMA_SetMemoryAddress(adc_dma, adc_dma_stream, (uint32_t)adc_dma_buffer);
        LL_DMA_SetDataLength(adc_dma, adc_dma_stream, ADC_DMA_BUFFER_SIZE);
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
        LL_DMA_EnableIT_HT(adc_dma, adc_dma_stream);
        LL_DMA_EnableIT_TC(adc_dma, adc_dma_stream);
        LL_DMA_EnableStream(adc_dma, adc_dma_stream);

    }
    if (isUseTimer)
        timer.Start(SampleRate);
    LL_ADC_REG_StartConversion(adc);
    return *this;
}

uint16_t ADC::Get_Data_Pollfor(void) {
    LL_ADC_REG_StartConversion(adc);
    while(LL_ADC_IsActiveFlag_EOC(adc) == RESET);
    LL_ADC_ClearFlag_EOC(adc);
    return LL_ADC_REG_ReadConversionData32(adc);
}

ADC &ADC::Stop(void) {
    if (isUseTimer)
        timer.Stop();

    LL_ADC_Disable(adc);
    //LL_ADC_EnableDeepPowerDown(adc);

    if (isUseDma)
    {
        LL_DMA_DisableIT_HT(adc_dma, adc_dma_stream);
        LL_DMA_DisableIT_TC(adc_dma, adc_dma_stream);
        LL_DMA_DisableStream(adc_dma, adc_dma_stream);
        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
    }
    return *this;
}
#if USE_IRQ_FOR_DATA_HANDLING
void ADC::Irq_DMA(void) {
    // 完成中断,当前 DMA 正在使用缓冲区的前半部分,用户可以操作后半部分。
    if(LL_DMA_IsEnabledIT_TC(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(adc_dma, adc_dma_stream))
    {
        // SCB_InvalidateDCache_by_Addr 的参数是 byte num: uint16_t -> (ADC_DMA_BUFFER_SIZE / 2 )* 2
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE);
        bufferADC.Puts((uint16_t *)(&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE/2);
        flag_finished = true;
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
    }
    // 半完成中断,当前 DMA 正在使用缓冲区的后半部分,用户可以操作前半部分。
    if(LL_DMA_IsEnabledIT_HT(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(adc_dma, adc_dma_stream))
    {
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE);
        bufferADC.Puts((uint16_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE/2);
        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
    }
}

#elif USE_POLLFOR_FOR_DATA_HANDLING
void ADC::Irq_DMA(void) {
    // 完成中断
    if(LL_DMA_IsEnabledIT_TC(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(adc_dma, adc_dma_stream))
    {
        // SCB_InvalidateDCache_by_Addr 的参数是 byte num: uint16_t -> (ADC_DMA_BUFFER_SIZE / 2 )* 2
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE);
        flag_finished = true;
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
    }
    // 半完成中断
    if(LL_DMA_IsEnabledIT_HT(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(adc_dma, adc_dma_stream))
    {

        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE);
        flag_halfFinished = true;
        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
    }
}
#elif USE_YOURSELF_DMAIRQ
#include "Machine.hpp"
float32_t Fir_out[1];
uint32_t Fir_count = 0;
float32_t adc_Voltage[1];
void ADC::Irq_DMA(void) {
    // 完成中断
    if(LL_DMA_IsEnabledIT_TC(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(adc_dma, adc_dma_stream))
    {
        // SCB_InvalidateDCache_by_Addr 的参数是 byte num: uint16_t -> (ADC_DMA_BUFFER_SIZE / 2 )* 2
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer), ADC_DMA_BUFFER_SIZE * 2);
        adc_Voltage[0] = adc_dma_buffer[0];
        FIR_Run_OnePoint(adc_Voltage, Fir_out);
        cout << Fir_count << ',' <<Fir_out[0] <<'\n';
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
        if (Fir_count++==1024)
            Stop();
        else
            LL_DMA_EnableStream(adc_dma, adc_dma_stream);
    }
//    // 半完成中断
//    if(LL_DMA_IsEnabledIT_HT(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(adc_dma, adc_dma_stream))
//    {
//
//        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE);
//        flag_halfFinished = true;
//        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
//    }
}
#endif




bool ADC::Is_Finished(void) {
    if (flag_finished) {
        flag_finished = false;
        return true;
    }
    return false;
}

bool ADC::Get_Data(uint16_t *buffer, uint32_t number) {
    if(bufferADC.Size() < number)//没有足够长的数据
        return false;
    else
    {
        bufferADC.Gets(buffer,number);//数据出队
        return true;
    }
}


uint32_t ADC::Get_DataSize(void) {
    return bufferADC.Size();
}

#if USE_POLLFOR_FOR_DATA_HANDLING
bool ADC::Scan_Data(void) {
    if (flag_halfFinished)
    {
        DISABLE_INT();
        flag_halfFinished = false;
        bufferADC.Puts((uint16_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE/2);
        ENABLE_INT();
    }
    else if (flag_finished)
    {
        DISABLE_INT();
        flag_finished = false;
        bufferADC.Puts((uint16_t *)(&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE/2);
        ENABLE_INT();
        return true;
    }
    return false;
}
#endif



