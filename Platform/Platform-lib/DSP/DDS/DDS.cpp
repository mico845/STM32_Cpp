//
// Created by Whisky on 1/12/2024.
//

#include "DDS.hpp"
#include "Platform-Operation/Platform-Operation-STM32_DMA.h"

DDS &DDS::Start(void) {
    pdac->Use_DMAIrq();
    pdac->Start(clkFreq);
    return *this;
}

DDS &DDS::Stop(void) {
    pdac->Stop();
    return *this;
}

void DDS::Irq_DMA(void) {
    // 完成中断，处理ROM数组后半段
    if(LL_DMA_IsEnabledIT_TC(pdac->dma, pdac->dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(pdac->dma, pdac->dma_stream))
    {
        for(uint32_t i = 0; i < (pdac->dac_dma_buffer.Size / 2); i++)
        {
            pdac->dac_dma_buffer.Data[ROM_Index] = pROM[P_WORD];
            ROM_Index++;

            P_WORD+=F_WORD;
            if (P_WORD >= ROM_Num)  //仅溢出后才进行取模运算，优化时间
                P_WORD %= ROM_Num;

            if (ROM_Index == pdac->dac_dma_buffer.Size)
                ROM_Index = 0;
        }
        Platform_DMA_ClearFlag_TC(pdac->dma, pdac->dma_stream);
    }
    // 半完成中断，处理ROM数组前半段
    if(LL_DMA_IsEnabledIT_HT(pdac->dma, pdac->dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(pdac->dma, pdac->dma_stream))
    {
        for(uint32_t i = 0; i < (pdac->dac_dma_buffer.Size / 2); i++)
        {
            pdac->dac_dma_buffer.Data[ROM_Index] = pROM[P_WORD];
            ROM_Index++;

            P_WORD+=F_WORD;
            if (P_WORD >= ROM_Num)
                P_WORD %= ROM_Num;

            if (ROM_Index == pdac->dac_dma_buffer.Size)
                ROM_Index = 0;
        }
        Platform_DMA_ClearFlag_HT(pdac->dma, pdac->dma_stream);
    }
}

DDS &DDS::Set_WaveROM(const uint16_t *Buffer, uint32_t Num) {
    pROM = Buffer;
    ROM_Num = Num;
    return *this;
}

DDS& DDS_Config(DDS& dds, uint64_t clkFreq,const uint16_t* Buffer, uint32_t Num, uint32_t Freq, float Phase)
{
    uint32_t FWORD = (double )Freq * Num / clkFreq;
    dds.Set_WaveROM(Buffer, Num);
    dds.Set_ClkFreq(clkFreq);
    dds.Set_FWORD(FWORD);
    dds.Set_PWORD((double )Phase/360 * Num);
    return dds;
}




