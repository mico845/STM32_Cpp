//
// Created by Whisky on 12/30/2023.
//

#ifndef CODE_IRQ_H
#define CODE_IRQ_H

#ifdef __cplusplus
#include "stm32h7xx_it.h"

extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/
/* CallBack */

void UART_DMA_TX_DONE_Callback(void);


/* IRQHandler */
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void TIM7_IRQHandler(void);
void ADC_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);
#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#endif

#endif //CODE_IRQ_H
