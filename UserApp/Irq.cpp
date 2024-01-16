//
// Created by Whisky on 12/30/2023.
//

#include "Irq/Irq.hpp"
#include "common_inc.h"

/* USART */
USART_IRQ(1, cout.Irq())
DMA_IRQ(1, 0, cout.Irq_RX_DMA())
DMA_IRQ(1, 1, cout.Irq_TX_DMA())

/* ADC */
DMA_IRQ(2, 0, adc.Irq_DMA())

/* DDS */
//DMA_IRQ(2, 1, dds.Irq_DMA())
/* DAC */
//DMA_IRQ(2, 1, dac.Irq_DMA())

/* TIM */
TIM_IRQ(7, timer.Irq())

