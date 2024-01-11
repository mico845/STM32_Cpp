//
// Created by Whisky on 1/2/2024.
//

#ifndef CODE_EXTI_H
#define CODE_EXTI_H

#include "OnChip/GPIO/GPIO.hpp"

#ifndef __TYPEDEF_GPIO_EXTI_CALLBACK__
    typedef void (*Platform_GPIOEXTI_CallBack_fun)(void);
#endif

class GPIO_EXTI
{
private:
    /* Variables */
    GPIO_TypeDef* port;
    uint32_t pin;
    uint32_t exti_line;
    Platform_GPIOEXTI_CallBack_fun callback_fun;
public:
    /* Variables */

    /* Constructor */
    GPIO_EXTI() = default;
    ~GPIO_EXTI() = default;

    GPIO_EXTI(GPIO_Port Port, uint8_t Pin, Platform_GPIOEXTI_CallBack_fun CallBack_fun = nullptr);
    /* Functions */
    GPIO_EXTI& RegisterCallback(Platform_GPIOEXTI_CallBack_fun CallBack_fun);
    GPIO_EXTI& UnRegisterCallback(void);
    bool Read(void);
    void Irq(void);
};

#endif //CODE_EXTI_H
