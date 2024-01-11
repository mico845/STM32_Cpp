//
// Created by Whisky on 1/10/2024.
//

#ifndef CODE_ADC_H
#define CODE_ADC_H
#include "OnChip/PWM/PWM.hpp"

#ifndef ADC_DMA_BUFFER_SIZE
    #define ADC_DMA_BUFFER_SIZE    (128)
#endif

class ADC
{
private:
    ADC_TypeDef* adc;
    PWM timer;
    bool isUseDma;
    bool isUseTimer;
    DMA_TypeDef* adc_dma;
    uint32_t adc_dma_stream;
    uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
    bool flag_finished;
    bool flag_first = true;
public:
    ADC() = default;
    ~ADC() = default;
    ADC(uint8_t ADC_ID,
        STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode = STM32::STM32_ADC::offset,
        STM32::STM32_ADC::ADC_SingleDiff SingleDiff = STM32::STM32_ADC::single);
    ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t TIMER_ID,
        STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode = STM32::STM32_ADC::offset,
        STM32::STM32_ADC::ADC_SingleDiff SingleDiff = STM32::STM32_ADC::single);
    ADC(uint8_t ADC_ID, uint8_t DMA_ID, uint8_t STREAM_ID, uint8_t PWM_TIMER_ID,  STM32::STM32_TIM::Timer_Channel PWM_Channel,
        STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode = STM32::STM32_ADC::offset,
        STM32::STM32_ADC::ADC_SingleDiff SingleDiff = STM32::STM32_ADC::single);

    ADC& Calibrate(STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode, STM32::STM32_ADC::ADC_SingleDiff SingleDiff);
    ADC& Start(uint32_t SampleRate);
    ADC& Stop(void);

    uint16_t Get_Data_Pollfor(void);    //支持无中断和DMA时使用

    void Irq_DMA(void);

    bool Is_Finished(void);
    uint16_t* Get_Data(void);
};

#endif //CODE_ADC_H
