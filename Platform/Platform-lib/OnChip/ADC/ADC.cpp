//
// Created by Whisky on 1/10/2024.
//

#include "ADC.hpp"
#include "Platform-Operation/Platform-Operation-STM32_DMA.h"
using namespace STM32::STM32_ADC;
using namespace STM32::STM32_DMA;
using namespace STM32::STM32_TIM;

ADC::ADC(uint8_t ADC_ID, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
         : adc(STM32::STM32_ADC::MapADC[ADC_ID]), isUseDma(false), isUseTimer(false), flag_finished(false)
{
    Calibrate(CalibrationMode, SingleDiff);     //开始校准
    while (LL_ADC_IsCalibrationOnGoing(adc));   //等待校准完成

    LL_ADC_SetBoostMode(adc, LL_ADC_BOOST_MODE_50MHZ);
}

ADC::ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t TIMER_ID, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
                :   adc(MapADC[ADC_ID]), adc_dma(MapDMA[DMA_ID]), adc_dma_stream(MapDMA_Stream[STREAM_ID]),
                    isUseDma(true), isUseTimer(true), flag_finished(false)
{
    ADC(ADC_ID, DMA_ID, STREAM_ID, TIMER_ID, channel1, CalibrationMode, SingleDiff);
}

ADC::ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t PWM_TIMER_ID, Timer_Channel PWM_Channel,
         STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode, STM32::STM32_ADC::ADC_SingleDiff SingleDiff)
                :   adc(MapADC[ADC_ID]), adc_dma(MapDMA[DMA_ID]), adc_dma_stream(MapDMA_Stream[STREAM_ID]),
                    isUseDma(true), isUseTimer(true), flag_finished(false)
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

void ADC::Irq_DMA(void) {
    // 完成中断
    if(LL_DMA_IsEnabledIT_TC(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(adc_dma, adc_dma_stream))
    {
        //保证地址能刷新到全地址
        if (flag_first)
        {
            SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE * 2);
            flag_first = false;
        }
        // byte num:uint16_t -> (ADC_DMA_BUFFER_SIZE / 2 )* 2
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE);
        flag_finished = true;
        Platform_DMA_ClearFlag_TC(adc_dma, adc_dma_stream);
    }
    // 半完成中断
    if(LL_DMA_IsEnabledIT_HT(adc_dma, adc_dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(adc_dma, adc_dma_stream))
    {

        SCB_InvalidateDCache_by_Addr((uint32_t *)(&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE);
        Platform_DMA_ClearFlag_HT(adc_dma, adc_dma_stream);
    }
}

bool ADC::Is_Finished(void) {
    if (flag_finished) {
        flag_finished = false;
        return true;
    }
    return false;
}

uint16_t *ADC::Get_Data(void) {
    return adc_dma_buffer;
}



