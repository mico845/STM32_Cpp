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

#define DMA_IRQ(__DMA__ , __DMA_STREAM__, __CALLBACK__)     void DMA##__DMA__##_Stream##__DMA_STREAM__##_IRQHandler(void){__CALLBACK__;}
#define TIM_IRQ(__TIMER__, __CALLBACK__)                    void TIM##__TIMER__##_IRQHandler(void){__CALLBACK__;}
#define USART_IRQ(__USART__, __CALLBACK__)                  void USART##__USART__##_IRQHandler(void){__CALLBACK__;}

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#endif

#endif //CODE_IRQ_H
