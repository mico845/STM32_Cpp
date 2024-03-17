#include "platform-drivers/STM32/EXTI/GPIO_EXTI.hpp"

using namespace STM32;

GPIO_EXTI::GPIO_EXTI(GPIO_TypeDef* Port, uint8_t Pin, Platform_GPIOEXTI_CallBack_fun CallBack_fun)
                            :port(Port), pin(STM32::MapGPIOPin[Pin]) , callback_fun(CallBack_fun)
{
    exti_line = (0x1UL << ((uint32_t)pin));
}

GPIO_EXTI& GPIO_EXTI::registerCallback(Platform_GPIOEXTI_CallBack_fun Callback_fun) {
    callback_fun = Callback_fun;
    return *this;
}

GPIO_EXTI& GPIO_EXTI::unregisterCallback(void) {
    callback_fun = nullptr;
    return *this;
}

void GPIO_EXTI::irq(void) {
    if (callback_fun != nullptr)
        callback_fun();
    if (LL_EXTI_IsActiveFlag_0_31(exti_line) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(exti_line);
    }
}

bool GPIO_EXTI::read(void) {
    return ((READ_BIT(port->IDR, pin) == (pin)) ? 1UL : 0UL);
}





