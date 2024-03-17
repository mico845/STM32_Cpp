#ifndef __PLATFORM_STM32_SPI_H__
#define __PLATFORM_STM32_SPI_H__

#include "platform-drivers/STM32/LL-support.h"

__STATIC_INLINE void SPI_SendBytes(SPI_TypeDef *spi, const uint8_t* data, uint16_t size)
{
    LL_SPI_SetHalfDuplexDirection(spi, LL_SPI_HALF_DUPLEX_TX);
    LL_SPI_SetTransferSize(spi, size);
    LL_SPI_Enable(spi);
    LL_SPI_StartMasterTransfer(spi);
    while (size > 0U)
    {
        if (LL_SPI_IsActiveFlag_TXP(spi))
        {
            LL_SPI_TransmitData8(spi, *((const uint8_t *)data));
            data += sizeof(uint8_t);
            size--;
        }
    }
    while (LL_SPI_IsActiveFlag_EOT(spi) == RESET);
    LL_SPI_ClearFlag_EOT(spi);
    LL_SPI_ClearFlag_TXTF(spi);
    LL_SPI_Disable(spi);
}

__STATIC_INLINE void SPI_SendByte(SPI_TypeDef *spi, const uint8_t data)
{
    SPI_SendBytes(spi, &data, 1);
}

#endif //__PLATFORM_STM32_SPI_H__
