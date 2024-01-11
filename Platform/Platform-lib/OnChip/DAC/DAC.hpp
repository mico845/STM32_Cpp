//
// Created by Whisky on 1/11/2024.
//

#ifndef HELLOWORLD_DAC_HPP
#define HELLOWORLD_DAC_HPP
#include "OnChip/Timer/Timer.hpp"
#include "DataStructre/Array.hpp"

#ifndef DAC_DMA_BUFFER_SIZE
    #define DAC_DMA_BUFFER_SIZE    (1024)
#endif

class DAC
{
private:
    /* Variables */
    DAC_TypeDef *dac;
    uint32_t channel;
    DMA_TypeDef *dma;
    uint32_t dma_stream;
    bool isUseDma;
    bool isUseDmaIrq;
    bool isUseTimer;
    Timer dac_timer;
    Array<uint16_t, DAC_DMA_BUFFER_SIZE> dac_dma_buffer;
public:
    /* Variables */

    /* Constructor */
    DAC() = default;
    ~DAC() = default;

    DAC(uint8_t DAC_ID, uint8_t DAC_Channel)
            :   dac(STM32::STM32_DAC::MapDAC[DAC_ID]), channel(STM32::STM32_DAC::MapDAC_Channel[DAC_Channel]),
                isUseDma(false), isUseTimer(false) { }
    DAC(uint8_t DAC_ID, uint8_t DAC_Channel, uint8_t DMA_ID, uint8_t DMA_STREAM, uint8_t TIMER_ID, bool IsUseDMA_Irq = false);

    /* Functions */
    DAC& Start(uint32_t Freq = 1000);
    DAC& Stop(void);

    void Set_Value(uint16_t Value);
    bool Set_DMABuffer(uint16_t* buffer, uint32_t size);

    void Irq_DMA(void);
};

#endif //HELLOWORLD_DAC_HPP
