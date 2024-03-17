#include "platform-drivers/STM32/Irq/Irq.hpp"
#include "common_inc.h"

using namespace STM32;

/* TIM */
TIM_IRQ(SYSTIM_TIM_ID)
{
    sysTimer.irq();
}

/* ADC */
DMA_IRQ(2, 0)
{
    adc.irq_DMA();
}

#if 0
/* DAC */
DMA_IRQ(2, 1)
{
    dac.Irq_DMA();
}
#else
/* DDS */
DMA_IRQ(2, 1)
{
    dds.irq_DMA();
}

#endif


/* USART */
#if 0
USART_IRQ(1)
{
    cout.Irq();
}
DMA_IRQ(1, 0)
{
    cout.Irq_RXDMA();
}
DMA_IRQ(1, 1)
{
    cout.Irq_TXDMA();
}
#endif


