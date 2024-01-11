//
// Created by Whisky on 1/7/2024.
//

#ifndef CODE_PLATFORM_OPERATION_STM32_SYS_H
#define CODE_PLATFORM_OPERATION_STM32_SYS_H
#include "cmsis_gcc.h"

#define ENABLE_INT()	__set_PRIMASK(0)
#define DISABLE_INT()	__set_PRIMASK(1)


#endif //CODE_PLATFORM_OPERATION_STM32_SYS_H
