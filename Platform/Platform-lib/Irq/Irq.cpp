//
// Created by Whisky on 12/30/2023.
//

#include "Irq.hpp"
#include "common_inc.h"

void DMA1_Stream0_IRQHandler(void)
{
    cout.Irq_RX_DMA();
}

void DMA1_Stream1_IRQHandler(void)
{
    cout.Irq_TX_DMA();
}

void USART1_IRQHandler(void)
{
    cout.Irq();
}

void DMA1_Stream2_IRQHandler(void)
{
    dds.Irq_DMA();
}

void TIM7_IRQHandler(void)
{
    timer.Irq();
}

void ADC_IRQHandler(void)
{

}
void ADC3_IRQHandler(void)
{

}

void DMA2_Stream0_IRQHandler(void)
{
    adc.Irq_DMA();
}

