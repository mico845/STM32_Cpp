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



#define _EXTI_IRQ(__EXTI__)                   void EXTI##__EXTI__##_IRQHandler(void)
/**
 * @brief EXTI中断服务函数 0-4
 *
 * @param __EXTI__ 引脚号
 */
#define EXTI_IRQ(__EXTI__)                     _EXTI_IRQ(__EXTI__)



#define _EXTI_IRQ9_5                           void EXTI9_5_IRQHandler(void)
/**
 * @brief EXTI中断服务函数 5-9
 *
 * @param __EXTI__ 引脚号
 */
#define EXTI_IRQ9_5                            _EXTI_IRQ9_5


#define _EXTI_IRQ15_10                           void EXTI15_10_IRQHandler(void)
/**
 * @brief EXTI中断服务函数 10-15
 *
 * @param __EXTI__ 引脚号
 */
#define EXTI_IRQ15_10                             _EXTI_IRQ15_10


#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#endif

#endif //__PLATFORM_STM32_IRQ_H__
