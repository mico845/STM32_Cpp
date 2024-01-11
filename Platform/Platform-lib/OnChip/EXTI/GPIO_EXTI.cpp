//
// Created by Whisky on 1/2/2024.
//

#include "GPIO_EXTI.hpp"
#include "Platform-Operation/Platform-Operation-STM32_GPIO.h"

using namespace STM32::STM32_EXTI;

GPIO_EXTI::GPIO_EXTI(GPIO_Port Port, uint8_t Pin, Platform_GPIOEXTI_CallBack_fun CallBack_fun)
                            : pin(MapPin[Pin]) , exti_line(MapDMA_EXTI_LINE[Pin]), callback_fun(CallBack_fun)
{
    port = Platform_GPIOPort_Convert(Port);
}

GPIO_EXTI& GPIO_EXTI::RegisterCallback(Platform_GPIOEXTI_CallBack_fun Callback_fun) {
    callback_fun = Callback_fun;
    return *this;
}

GPIO_EXTI& GPIO_EXTI::UnRegisterCallback(void) {
    callback_fun = nullptr;
    return *this;
}

void GPIO_EXTI::Irq(void) {
    if (callback_fun != nullptr)
        callback_fun();
    if (LL_EXTI_IsActiveFlag_0_31(exti_line) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(exti_line);
    }
}

bool GPIO_EXTI::Read(void) {
    return ((READ_BIT(port->IDR, pin) == (pin)) ? 1UL : 0UL);
}





