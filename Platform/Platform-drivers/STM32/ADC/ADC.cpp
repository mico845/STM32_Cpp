#include "platform-drivers/STM32/ADC/ADC.hpp"
#include "platform-drivers/STM32/System/Sys.hpp"

using namespace STM32;

ADC::ADC(ADC_TypeDef* ADC, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
         : adc(ADC), isUseDma(false), isUseTimer(false)
{
    calibrate(CalibrationMode, SingleDiff);     //开始校准
    while (LL_ADC_IsCalibrationOnGoing(adc));   //等待校准完成

    LL_ADC_SetBoostMode(adc, LL_ADC_BOOST_MODE_50MHZ);
}

ADC::ADC(ADC_TypeDef* ADCx, DMA_TypeDef* ADC_DMA, DMA_Stream ADC_DMA_Stream, TIM_TypeDef* TIMx, ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
                :   adc(ADCx), adc_dma(ADC_DMA), adc_dma_stream(ADC_DMA_Stream),
                    isUseDma(true), isUseTimer(true)
{
    ADC(ADCx, ADC_DMA, ADC_DMA_Stream, TIMx, channel1, CalibrationMode, SingleDiff);
}

ADC::ADC(ADC_TypeDef* ADCx, DMA_TypeDef* ADC_DMA, DMA_Stream ADC_DMA_Stream, TIM_TypeDef* PWMx, Timer_Channel PWM_Channel,
         ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff)
                :   adc(ADCx), adc_dma(ADC_DMA), adc_dma_stream(ADC_DMA_Stream),
                    isUseDma(true), isUseTimer(true)
{
    timer = PWM(PWMx, PWM_Channel);

    calibrate(CalibrationMode, SingleDiff);     //开始校准
    while (LL_ADC_IsCalibrationOnGoing(adc));   //等待校准完成

    LL_ADC_REG_SetDataTransferMode(adc, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    LL_ADC_SetBoostMode(adc, LL_ADC_BOOST_MODE_50MHZ);
}

ADC &ADC::calibrate(ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff) {
    MODIFY_REG(adc->CR,
               ADC_CR_ADCALLIN | ADC_CR_ADCALDIF | ADC_CR_BITS_PROPERTY_RS,
               ADC_CR_ADCAL | (CalibrationMode & ADC_CALIB_MODE_MASK) | (SingleDiff & ADC_SINGLEDIFF_CALIB_START_MASK));
    return *this;
}

ADC &ADC::start(uint32_t SampleRate) {
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
        timer.start(SampleRate);
    LL_ADC_REG_StartConversion(adc);
    return *this;
}

uint16_t ADC::getData_Pollfor(void) {
    LL_ADC_REG_StartConversion(adc);
    while(LL_ADC_IsActiveFlag_EOC(adc) == RESET);
    LL_ADC_ClearFlag_EOC(adc);
    return LL_ADC_REG_ReadConversionData32(adc);
}

ADC &ADC::stop(void) {
    if (isUseTimer)
        timer.stop();

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

void ADC::irq_DMA(void) {
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

bool ADC::isFinished(void) {
    if (flag_finished) {
        flag_finished = false;
        return true;
    }
    return false;
}

bool ADC::getData(uint16_t *buffer, uint32_t number) {
    if(bufferADC.size() < number)//没有足够长的数据
        return false;
    else
    {
        bufferADC.gets(buffer, number);//数据出队
        return true;
    }
}

uint32_t ADC::getDataSize(void) {
    return bufferADC.size();
}

#if USE_POLLFOR_FOR_DATA_HANDLING

bool ADC::scanData(void) {
    if (flag_halfFinished)
    {
        DISABLE_INT();
        flag_halfFinished = false;
        bufferADC.puts((uint16_t *) (&adc_dma_buffer[0]), ADC_DMA_BUFFER_SIZE / 2);
        ENABLE_INT();
    }
    else if (flag_finished)
    {
        DISABLE_INT();
        flag_finished = false;
        bufferADC.puts((uint16_t *) (&adc_dma_buffer[ADC_DMA_BUFFER_SIZE / 2]), ADC_DMA_BUFFER_SIZE / 2);
        ENABLE_INT();
        return true;
    }
    return false;
}

#endif



