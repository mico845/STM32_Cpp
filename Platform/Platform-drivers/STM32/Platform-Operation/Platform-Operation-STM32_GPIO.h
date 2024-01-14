//
// Created by Whisky on 1/2/2024.
//

#ifndef CODE_PLATFORM_OPERATION_STM32_GPIO_H
#define CODE_PLATFORM_OPERATION_STM32_GPIO_H

#include "Platform-STM32.h"

__STATIC_INLINE  GPIO_TypeDef* Platform_GPIOPort_Convert(STM32::STM32_GPIO::GPIO_Port Port) {
    if (Port == STM32::STM32_GPIO::A)
        return GPIOA;
    else if (Port == STM32::STM32_GPIO::B)
        return GPIOB;
    else if (Port == STM32::STM32_GPIO::C)
        return GPIOC;
    else if (Port == STM32::STM32_GPIO::D)
        return GPIOD;
    else if (Port == STM32::STM32_GPIO::E)
        return GPIOE;
    else if (Port == STM32::STM32_GPIO::F)
        return GPIOF;
    else if (Port == STM32::STM32_GPIO::G)
        return GPIOG;
    else if (Port == STM32::STM32_GPIO::H)
        return GPIOH;
    else
        return nullptr;
}

#endif //CODE_PLATFORM_OPERATION_STM32_GPIO_H
