#ifndef __PLATFORM_STM32_IRQ_H__
#define __PLATFORM_STM32_IRQ_H__

#ifdef __cplusplus
#include "platform-drivers/STM32/LL-support.h"
#include __STM32_IT_H__

extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/


#define _DMA_IRQ(__DMA__ , __DMA_STREAM__)      void DMA##__DMA__##_Stream##__DMA_STREAM__##_IRQHandler(void)
/**
 * @brief DMA中断服务函数
 *
 * @param __DMA__ DMA号
 * @param __DMA_STREAM__ DMA流号
 */
#define DMA_IRQ(__DMA__ , __DMA_STREAM__)       _DMA_IRQ(__DMA__ , __DMA_STREAM__)

#define _TIM_IRQ(__TIMER__)                     void TIM##__TIMER__##_IRQHandler(void)
/**
 * @brief 定时器中断服务函数
 *
 * @param __TIMER__ 定时器号
 */
#define TIM_IRQ(__TIMER__)                      _TIM_IRQ(__TIMER__)


#define _USART_IRQ(__USART__)                   void USART##__USART__##_IRQHandler(void)
/**
 * @brief 串口中断服务函数
 *
 * @param __USART__ 串口号
 */
#define USART_IRQ(__USART__)                    _USART_IRQ(__USART__)




#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#endif

#endif //__PLATFORM_STM32_IRQ_H__
