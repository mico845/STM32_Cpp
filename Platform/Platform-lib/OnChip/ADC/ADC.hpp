//
// Created by Whisky on 1/10/2024.
//

#ifndef CODE_ADC_H
#define CODE_ADC_H
#include "OnChip/PWM/PWM.hpp"
#include "DataStructre/Fifo.hpp"

#define USE_IRQ_FOR_DATA_HANDLING       (0)         //在中断服务函数里进行数据搬运（双缓冲）
#define USE_POLLFOR_FOR_DATA_HANDLING   (1)         //在CPU的轮询函数里进行数据搬运（双缓冲）

#ifndef ADC_BUFFER_SIZE
    #define ADC_BUFFER_SIZE        (1024)                    //ADC BUFFER FIFO SIZE
#endif

#ifndef ADC_DMA_BUFFER_SIZE
    #define ADC_DMA_BUFFER_SIZE    (ADC_BUFFER_SIZE)
#endif

class ADC
{
private:
    /* Variables */
    ADC_TypeDef* adc;
    PWM timer;
    bool isUseDma;
    bool isUseTimer;
    DMA_TypeDef* adc_dma;
    uint32_t adc_dma_stream;
    uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
    Fifo<uint16_t, ADC_BUFFER_SIZE> bufferADC;
    bool flag_finished = false;
    bool flag_halfFinished = false;
public:
    /* Variables */

    /* Constructor */
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

    /* Functions */
    ADC& Calibrate(STM32::STM32_ADC::ADC_CalibrationMode CalibrationMode, STM32::STM32_ADC::ADC_SingleDiff SingleDiff);
    ADC& Start(uint32_t SampleRate);
    ADC& Stop(void);

    uint16_t Get_Data_Pollfor(void);    //支持无中断和DMA时使用

    void Irq_DMA(void);

    bool Is_Finished(void);
    bool Get_Data(uint16_t *buffer, uint32_t number);
    uint32_t Get_DataSize(void);

#if USE_POLLFOR_FOR_DATA_HANDLING
    bool Scan_Data(void);
#endif

};

#endif //CODE_ADC_H
