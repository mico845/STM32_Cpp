#ifndef __PLATFORM_STM32_DMA_H__
#define __PLATFORM_STM32_DMA_H__

#include "platform-drivers/STM32/LL-support.h"

namespace STM32
{
    /**
     * @brief DMA数据流
     *
     * DMA数据流。
     */
    typedef enum
    {
        DMA_Stream_0    =       0x00000000U,
        DMA_Stream_1    =       0x00000001U,
        DMA_Stream_2    =       0x00000002U,
        DMA_Stream_3    =       0x00000003U,
        DMA_Stream_4    =       0x00000004U,
        DMA_Stream_5    =       0x00000005U,
        DMA_Stream_6    =       0x00000006U,
        DMA_Stream_7    =       0x00000007U,
        DMA_Stream_ALL  =       0xFFFF0000U,
    }DMA_Stream;
}

/**
 * @brief 检查DMA传输半完成中断标志位是否已设置
 */
#define PLATFORM_DMA_IsActiveFlag_HT(dma, dma_stream) \
                    ((dma_stream == STM32::DMA_Stream_0) ? LL_DMA_IsActiveFlag_HT0(dma) : \
                    ((dma_stream == STM32::DMA_Stream_1) ? LL_DMA_IsActiveFlag_HT1(dma) : \
                    ((dma_stream == STM32::DMA_Stream_2) ? LL_DMA_IsActiveFlag_HT2(dma) : \
                    ((dma_stream == STM32::DMA_Stream_3) ? LL_DMA_IsActiveFlag_HT3(dma) : \
                    ((dma_stream == STM32::DMA_Stream_4) ? LL_DMA_IsActiveFlag_HT4(dma) : \
                    ((dma_stream == STM32::DMA_Stream_5) ? LL_DMA_IsActiveFlag_HT5(dma) : \
                    ((dma_stream == STM32::DMA_Stream_6) ? LL_DMA_IsActiveFlag_HT6(dma) : \
                    ((dma_stream == STM32::DMA_Stream_7) ? LL_DMA_IsActiveFlag_HT7(dma) : 0))))))))

/**
 * @brief 检查DMA传输完成标志位是否已设置
 */
#define PLATFORM_DMA_IsActiveFlag_TC(dma, dma_stream) \
                    ((dma_stream == STM32::DMA_Stream_0) ? LL_DMA_IsActiveFlag_TC0(dma) : \
                    ((dma_stream == STM32::DMA_Stream_1) ? LL_DMA_IsActiveFlag_TC1(dma) : \
                    ((dma_stream == STM32::DMA_Stream_2) ? LL_DMA_IsActiveFlag_TC2(dma) : \
                    ((dma_stream == STM32::DMA_Stream_3) ? LL_DMA_IsActiveFlag_TC3(dma) : \
                    ((dma_stream == STM32::DMA_Stream_4) ? LL_DMA_IsActiveFlag_TC4(dma) : \
                    ((dma_stream == STM32::DMA_Stream_5) ? LL_DMA_IsActiveFlag_TC5(dma) : \
                    ((dma_stream == STM32::DMA_Stream_6) ? LL_DMA_IsActiveFlag_TC6(dma) : \
                    ((dma_stream == STM32::DMA_Stream_7) ? LL_DMA_IsActiveFlag_TC7(dma) : 0))))))))

/**
 * @brief 清除DMA传输半完成中断标志位
 */
__STATIC_INLINE void Platform_DMA_ClearFlag_HT(DMA_TypeDef * dma, STM32::DMA_Stream dma_stream) {
    switch (dma_stream) {
        case STM32::DMA_Stream_0:
            LL_DMA_ClearFlag_HT0(dma);
            break;
        case STM32::DMA_Stream_1:
            LL_DMA_ClearFlag_HT1(dma);
            break;
        case STM32::DMA_Stream_2:
            LL_DMA_ClearFlag_HT2(dma);
            break;
        case STM32::DMA_Stream_3:
            LL_DMA_ClearFlag_HT3(dma);
            break;
        case STM32::DMA_Stream_4:
            LL_DMA_ClearFlag_HT4(dma);
            break;
        case STM32::DMA_Stream_5:
            LL_DMA_ClearFlag_HT5(dma);
            break;
        case STM32::DMA_Stream_6:
            LL_DMA_ClearFlag_HT6(dma);
            break;
        case STM32::DMA_Stream_7:
            LL_DMA_ClearFlag_HT7(dma);
            break;
        default:
            // 可以选择在这里处理未知的 DMA Stream 值
            break;
    }
}

/**
 * @brief 清除DMA传输完成中断标志位
 */
__STATIC_INLINE void Platform_DMA_ClearFlag_TC(DMA_TypeDef * dma, STM32::DMA_Stream dma_stream) {
    switch (dma_stream) {
        case STM32::DMA_Stream_0:
            LL_DMA_ClearFlag_TC0(dma);
            break;
        case STM32::DMA_Stream_1:
            LL_DMA_ClearFlag_TC1(dma);
            break;
        case STM32::DMA_Stream_2:
            LL_DMA_ClearFlag_TC2(dma);
            break;
        case STM32::DMA_Stream_3:
            LL_DMA_ClearFlag_TC3(dma);
            break;
        case STM32::DMA_Stream_4:
            LL_DMA_ClearFlag_TC4(dma);
            break;
        case STM32::DMA_Stream_5:
            LL_DMA_ClearFlag_TC5(dma);
            break;
        case STM32::DMA_Stream_6:
            LL_DMA_ClearFlag_TC6(dma);
            break;
        case STM32::DMA_Stream_7:
            LL_DMA_ClearFlag_TC7(dma);
            break;
        default:
            // 可以选择在这里处理未知的 DMA Stream 值
            break;
    }
} //namespace STM32

#endif // !__PLATFORM_STM32_DMA_H__