//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_Platform_OPERATION_DMA_H
#define CODE_Platform_OPERATION_DMA_H
#include "Platform-STM32.h"

#define PLATFORM_DMA_IsActiveFlag_HT(dma, dma_stream) \
                    ((dma_stream == LL_DMA_STREAM_0) ? LL_DMA_IsActiveFlag_HT0(dma) : \
                    ((dma_stream == LL_DMA_STREAM_1) ? LL_DMA_IsActiveFlag_HT1(dma) : \
                    ((dma_stream == LL_DMA_STREAM_2) ? LL_DMA_IsActiveFlag_HT2(dma) : \
                    ((dma_stream == LL_DMA_STREAM_3) ? LL_DMA_IsActiveFlag_HT3(dma) : \
                    ((dma_stream == LL_DMA_STREAM_4) ? LL_DMA_IsActiveFlag_HT4(dma) : \
                    ((dma_stream == LL_DMA_STREAM_5) ? LL_DMA_IsActiveFlag_HT5(dma) : \
                    ((dma_stream == LL_DMA_STREAM_6) ? LL_DMA_IsActiveFlag_HT6(dma) : \
                    ((dma_stream == LL_DMA_STREAM_7) ? LL_DMA_IsActiveFlag_HT7(dma) : 0))))))))

#define PLATFORM_DMA_IsActiveFlag_TC(dma, dma_stream) \
                    ((dma_stream == LL_DMA_STREAM_0) ? LL_DMA_IsActiveFlag_TC0(dma) : \
                    ((dma_stream == LL_DMA_STREAM_1) ? LL_DMA_IsActiveFlag_TC1(dma) : \
                    ((dma_stream == LL_DMA_STREAM_2) ? LL_DMA_IsActiveFlag_TC2(dma) : \
                    ((dma_stream == LL_DMA_STREAM_3) ? LL_DMA_IsActiveFlag_TC3(dma) : \
                    ((dma_stream == LL_DMA_STREAM_4) ? LL_DMA_IsActiveFlag_TC4(dma) : \
                    ((dma_stream == LL_DMA_STREAM_5) ? LL_DMA_IsActiveFlag_TC5(dma) : \
                    ((dma_stream == LL_DMA_STREAM_6) ? LL_DMA_IsActiveFlag_TC6(dma) : \
                    ((dma_stream == LL_DMA_STREAM_7) ? LL_DMA_IsActiveFlag_TC7(dma) : 0))))))))



__STATIC_INLINE void Platform_DMA_ClearFlag_HT(DMA_TypeDef * dma, uint32_t dma_stream) {
    switch (dma_stream) {
        case LL_DMA_STREAM_0:
            LL_DMA_ClearFlag_HT0(dma);
            break;
        case LL_DMA_STREAM_1:
            LL_DMA_ClearFlag_HT1(dma);
            break;
        case LL_DMA_STREAM_2:
            LL_DMA_ClearFlag_HT2(dma);
            break;
        case LL_DMA_STREAM_3:
            LL_DMA_ClearFlag_HT3(dma);
            break;
        case LL_DMA_STREAM_4:
            LL_DMA_ClearFlag_HT4(dma);
            break;
        case LL_DMA_STREAM_5:
            LL_DMA_ClearFlag_HT5(dma);
            break;
        case LL_DMA_STREAM_6:
            LL_DMA_ClearFlag_HT6(dma);
            break;
        case LL_DMA_STREAM_7:
            LL_DMA_ClearFlag_HT7(dma);
            break;
        default:
            // 可以选择在这里处理未知的 DMA Stream 值
            break;
    }
}


__STATIC_INLINE void Platform_DMA_ClearFlag_TC(DMA_TypeDef * dma, uint32_t dma_stream) {
    switch (dma_stream) {
        case LL_DMA_STREAM_0:
            LL_DMA_ClearFlag_TC0(dma);
            break;
        case LL_DMA_STREAM_1:
            LL_DMA_ClearFlag_TC1(dma);
            break;
        case LL_DMA_STREAM_2:
            LL_DMA_ClearFlag_TC2(dma);
            break;
        case LL_DMA_STREAM_3:
            LL_DMA_ClearFlag_TC3(dma);
            break;
        case LL_DMA_STREAM_4:
            LL_DMA_ClearFlag_TC4(dma);
            break;
        case LL_DMA_STREAM_5:
            LL_DMA_ClearFlag_TC5(dma);
            break;
        case LL_DMA_STREAM_6:
            LL_DMA_ClearFlag_TC6(dma);
            break;
        case LL_DMA_STREAM_7:
            LL_DMA_ClearFlag_TC7(dma);
            break;
        default:
            // 可以选择在这里处理未知的 DMA Stream 值
            break;
    }
}


#endif //CODE_Platform_OPERATION_DMA_H
