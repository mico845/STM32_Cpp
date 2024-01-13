//
// Created by Whisky on 1/11/2024.
//

#ifndef HELLOWORLD_DAC_HPP
#define HELLOWORLD_DAC_HPP
#include "OnChip/Timer/Timer.hpp"
#include "DataStructre/Array.hpp"

#ifndef DAC_DMA_BUFFER_MAX_SIZE
    #define DAC_DMA_BUFFER_MAX_SIZE    (256)
#endif

class DAC
{
public:
    friend class DDS;
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
    Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE> dac_dma_buffer;
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

    __INLINE void Set_Value(uint16_t Value);
    bool Set_DMABuffer(const uint16_t* Buffer, uint32_t Num);
    void Enable_DMAIrq(void);
    void Disable_DMAIrq(void);
    __INLINE void Use_DMAIrq(void);
    __INLINE  void Unuse_DMAIrq(void);

    inline Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE>* Get_PointBufArray(void);

    void Irq_DMA(void);
};

__INLINE void DAC::Set_Value(uint16_t Value) {
    LL_DAC_ConvertData12RightAligned(dac, channel, Value);  //在数据保存寄存器中设置要加载的数据
}

__INLINE void DAC::Use_DMAIrq(void) {
    isUseDmaIrq = true;
}

__INLINE void DAC::Unuse_DMAIrq(void) {
    isUseDmaIrq = false;
}

__INLINE Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE> *DAC::Get_PointBufArray(void) {
    return &dac_dma_buffer;
}


#endif //HELLOWORLD_DAC_HPP
