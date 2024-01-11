//
// Created by Whisky on 1/11/2024.
//

#include "DAC.hpp"
#include "Platform-Operation/Platform-Operation-STM32_DMA.h"

DAC::DAC(uint8_t DAC_ID, uint8_t DAC_Channel, uint8_t DMA_ID, uint8_t DMA_STREAM, uint8_t TIMER_ID, bool IsUseDMA_Irq)
        :   dac(STM32::STM32_DAC::MapDAC[DAC_ID]), channel(STM32::STM32_DAC::MapDAC_Channel[DAC_Channel]),
            dma(STM32::STM32_DMA::MapDMA[DMA_ID]), dma_stream(STM32::STM32_DMA::MapDMA_Stream[DMA_STREAM]),
            isUseDma(true), isUseTimer(true), isUseDmaIrq(IsUseDMA_Irq)
{
    dac_timer = Timer(TIMER_ID);
}

DAC &DAC::Start(uint32_t Freq) {
    /*
     * DAC 通道 1 触发使能 (DAC channel1 trigger enable)
     * 此位由软件置 1 和清零，以使能/禁止 DAC 通道 1 触发。
     * 0：禁止 DAC 通道 1 触发，写入 DACx_DHR1 寄存器的数据在一个 dac_pclk 时钟周期之后转移到 DACx_DOR1 寄存器
     * 1：使能 DAC 通道 1 触发，DACx_DHR1 寄存器的数据在三个 dac_pclk 时钟周期之后转移到 DACx_DOR1 寄存器
     * 注： 如果选择软件触发，DACx_DHR1 寄存器的内容只需一个 dac_pclk 时钟周期即可转移到DACx_DOR1 寄存器。
     */
    if (!isUseDma && !isUseTimer)
        LL_DAC_DisableTrigger(dac, channel);// 故选择写入 DACx_DHRx 寄存器的数据在一个 dac_pclk 时钟周期之后转移到 DACx_DORx 寄存器
    LL_DAC_Enable(dac, channel);                            //启动DAC选择的通道
    if (isUseDma)
    {
        LL_DMA_SetPeriphAddress(dma, dma_stream , LL_DAC_DMA_GetRegAddr(dac, channel, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED));
        LL_DAC_EnableDMAReq(dac, channel);
        LL_DMA_SetMemoryAddress(dma, dma_stream, (uint32_t)(dac_dma_buffer.Data));
        LL_DMA_SetDataLength(dma, dma_stream, dac_dma_buffer.Size);
        if (isUseDmaIrq)
        {
            Platform_DMA_ClearFlag_HT(dma, dma_stream);
            Platform_DMA_ClearFlag_TC(dma, dma_stream);
            LL_DMA_EnableIT_HT(dma, dma_stream);
            LL_DMA_EnableIT_TC(dma, dma_stream);
        }
        LL_DMA_EnableStream(dma, dma_stream);
    }
    else
        LL_DAC_ConvertData12RightAligned(dac, channel, 0);
    if (isUseTimer)
        dac_timer.Start(round((float )Freq * dac_dma_buffer.Size) );
    return *this;
}

DAC &DAC::Stop(void) {
    if (isUseTimer)
        dac_timer.Stop();
    LL_DAC_Disable(dac, channel);
    
    if (isUseDma)
    {
        LL_DMA_DisableStream(dma, dma_stream);
        if (isUseDmaIrq)
        {
            LL_DMA_DisableIT_HT(dma, dma_stream);
            LL_DMA_DisableIT_TC(dma, dma_stream);
            Platform_DMA_ClearFlag_HT(dma, dma_stream);
            Platform_DMA_ClearFlag_TC(dma, dma_stream);
        }
    }
    return *this;
}

void DAC::Set_Value(uint16_t Value) {
    LL_DAC_ConvertData12RightAligned(dac, channel, Value);  //在数据保存寄存器中设置要加载的数据
}


bool DAC::Set_DMABuffer(uint16_t* buffer, uint32_t size) {
    if (size > DAC_DMA_BUFFER_SIZE)
        return false;
    dac_dma_buffer.SetBuffer(buffer, size);
    return true;
}


void DAC::Irq_DMA(void) {
    // 完成中断
    if(LL_DMA_IsEnabledIT_TC(dma, dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(dma, dma_stream))
    {
        // SCB_InvalidateDCache_by_Addr 的参数是 byte num: uint16_t -> (DMA_BUFFER_SIZE / 2 )* 2
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&dac_dma_buffer[DAC_DMA_BUFFER_SIZE / 2]), DAC_DMA_BUFFER_SIZE);
        Platform_DMA_ClearFlag_TC(dma, dma_stream);
    }
    // 半完成中断
    if(LL_DMA_IsEnabledIT_HT(dma, dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(dma, dma_stream))
    {
        SCB_InvalidateDCache_by_Addr((uint32_t *)(&dac_dma_buffer[0]), DAC_DMA_BUFFER_SIZE);
        Platform_DMA_ClearFlag_HT(dma, dma_stream);
    }
}

