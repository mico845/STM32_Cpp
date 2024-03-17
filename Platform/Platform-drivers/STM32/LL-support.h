#ifndef __PLATFORM_LL_SUPPORT_H__
#define __PLATFORM_LL_SUPPORT_H__

#define PLATFORM_USE_STM_H7

#ifdef PLATFORM_USE_STM_H7

#define __STM32_IT_H__ "stm32h7xx_it.h"

/* STM32 H7 Includes */
#include "stm32h7xx.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_dac.h"
#include "stm32h7xx_ll_spi.h"

#endif // !PLATFORM_USE_STM_H7

#endif // !__PLATFORM_LL_SUPPORT_H__

