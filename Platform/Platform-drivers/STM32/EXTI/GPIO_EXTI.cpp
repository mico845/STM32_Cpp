#include "platform-drivers/STM32/EXTI/GPIO_EXTI.hpp"

using namespace STM32;

static const uint32_t Map_SYSCFG_EXTI_LINE[PLATFORM_GPIO_PIN_NUM] = { LL_SYSCFG_EXTI_LINE0, LL_SYSCFG_EXTI_LINE1, LL_SYSCFG_EXTI_LINE2, LL_SYSCFG_EXTI_LINE3, LL_SYSCFG_EXTI_LINE4,
                                                            LL_SYSCFG_EXTI_LINE5, LL_SYSCFG_EXTI_LINE6, LL_SYSCFG_EXTI_LINE7, LL_SYSCFG_EXTI_LINE8, LL_SYSCFG_EXTI_LINE9,
                                                            LL_SYSCFG_EXTI_LINE10, LL_SYSCFG_EXTI_LINE11, LL_SYSCFG_EXTI_LINE12, LL_SYSCFG_EXTI_LINE13, LL_SYSCFG_EXTI_LINE14,
                                                            LL_SYSCFG_EXTI_LINE15 };

static IRQn_Type GetEXTI_PIN_IRQn(uint8_t Pin) {
    if (Pin == 0)
        return EXTI0_IRQn;
    else if (Pin == 1)
        return EXTI1_IRQn;
    else if (Pin == 2)
        return EXTI2_IRQn;
    else if (Pin == 3)
        return EXTI3_IRQn;
    else if (Pin == 4)
        return EXTI4_IRQn;
    else if (Pin >= 5 && Pin <= 9)
        return EXTI9_5_IRQn;
    else
        return EXTI15_10_IRQn;
}

static uint8_t Platform_Get_SYSCFG_EXTI_PORT(GPIO_TypeDef* Port)
{
    if (Port == GPIOA)
        return LL_SYSCFG_EXTI_PORTA;
    else if (Port == GPIOB)
        return LL_SYSCFG_EXTI_PORTB;
    else if (Port == GPIOC)
        return LL_SYSCFG_EXTI_PORTC;
    else if (Port == GPIOD)
        return LL_SYSCFG_EXTI_PORTD;
    else if (Port == GPIOE)
        return LL_SYSCFG_EXTI_PORTE;
    else if (Port == GPIOG)
        return LL_SYSCFG_EXTI_PORTG;
    else if (Port == GPIOH)
        return LL_SYSCFG_EXTI_PORTH;
#if defined(GPIOI)
    else if (Port == GPIOI)
        return LL_SYSCFG_EXTI_PORTI;
#endif /*GPIOI*/
    else if (Port == GPIOJ)
        return LL_SYSCFG_EXTI_PORTJ;
    else
        return LL_SYSCFG_EXTI_PORTK;
}

uint32_t Platform_Get_PinPosition(uint32_t bs_value) {
    uint32_t position = 0;

    for (uint32_t i = 0; i < 16; i++) {
        uint32_t mask = 1UL << i;

        if ((bs_value & mask) == mask) {
            position = i;
            return position;
        }
    }
    return position;
}

GPIO_EXTI::GPIO_EXTI(GPIO_TypeDef* Port, uint8_t Pin, GPIO_Pull pull,EXTI_Trigger trigger, Platform_GPIOEXTI_CallBack_fun CallBack_fun, uint32_t PreemptPriority, uint32_t SubPriority)
                            : callback_fun(CallBack_fun)
{
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
    exti_line = (0x1UL << ((uint32_t)Pin));
    gpio = GPIO(Port, Pin, pull, in);

    LL_SYSCFG_SetEXTISource( Platform_Get_SYSCFG_EXTI_PORT(Port), Map_SYSCFG_EXTI_LINE[Pin]);

    /**/
    EXTI_InitStruct.Line_0_31 = exti_line;
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.Line_64_95 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = trigger;
    LL_EXTI_Init(&EXTI_InitStruct);

    /* EXTI interrupt init*/
    NVIC_SetPriority(GetEXTI_PIN_IRQn(Pin), NVIC_EncodePriority(NVIC_GetPriorityGrouping(),PreemptPriority, SubPriority));

}

GPIO_EXTI::GPIO_EXTI(GPIO EXTI_GPIO, EXTI_Trigger trigger, Platform_GPIOEXTI_CallBack_fun CallBack_fun,
                     uint32_t PreemptPriority, uint32_t SubPriority) : gpio(EXTI_GPIO), callback_fun(CallBack_fun)
{
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
    exti_line = (0x1UL << Platform_Get_PinPosition(gpio.pin));
    LL_SYSCFG_SetEXTISource( Platform_Get_SYSCFG_EXTI_PORT(gpio.port), Map_SYSCFG_EXTI_LINE[Platform_Get_PinPosition(gpio.pin)]);

    /**/
    EXTI_InitStruct.Line_0_31 = exti_line;
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.Line_64_95 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = trigger;
    LL_EXTI_Init(&EXTI_InitStruct);

    /**/
    LL_GPIO_SetPinMode(gpio.port, (uint32_t)gpio.pin, in);

    /* EXTI interrupt init*/
    NVIC_SetPriority(GetEXTI_PIN_IRQn(Platform_Get_PinPosition(gpio.pin)), NVIC_EncodePriority(NVIC_GetPriorityGrouping(),PreemptPriority, SubPriority));

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
    if (LL_EXTI_IsActiveFlag_0_31(exti_line) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(exti_line);
    }

    if (callback_fun != nullptr)
        callback_fun();
}

bool GPIO_EXTI::read(void) {
    return gpio.read();
}

uint32_t GPIO_EXTI::getExtiLine(void) {
    return exti_line;
}

void GPIO_EXTI::enableIT(void) {
    NVIC_EnableIRQ(GetEXTI_PIN_IRQn(Platform_Get_PinPosition(gpio.pin)));
}

void GPIO_EXTI::disableIT(void) {
    NVIC_DisableIRQ(GetEXTI_PIN_IRQn(Platform_Get_PinPosition(gpio.pin)));
}






