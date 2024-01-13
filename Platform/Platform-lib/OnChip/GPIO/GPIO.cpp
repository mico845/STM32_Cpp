//
// Created by Whisky on 12/29/2023.
//

#include "GPIO.hpp"
#include "Platform-Operation/Platform-Operation-STM32_GPIO.h"

GPIO::GPIO(GPIO_Port Port, uint8_t Pin, GPIO_Mode Mode, GPIO_OutPutType PutType, GPIO_Pull Pull, GPIO_Speed Speed)
                : pin(MapPin[Pin])

{
    port = Platform_GPIOPort_Convert(Port);
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    RCC_Configuration();

    WRITE_REG(port->BSRR, MapPin[Pin] << 16U);

    GPIO_InitStruct.Pin = MapPin[Pin];
    GPIO_InitStruct.Mode = Mode;
    GPIO_InitStruct.Speed = Speed;
    GPIO_InitStruct.OutputType = PutType;
    GPIO_InitStruct.Pull =  Pull;

    LL_GPIO_Init(port, &GPIO_InitStruct);
}

void GPIO::RCC_Configuration(void) {
    if(port==GPIOA)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    else if(port==GPIOB)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
    else if(port==GPIOC)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    else if(port==GPIOD)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
    else if(port==GPIOE)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
    else if(port==GPIOF)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
    else if(port==GPIOG)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
    else if(port==GPIOH)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOH);
}

void GPIO::SetConfig(GPIO_Mode Mode, GPIO_OutPutType PutType, GPIO_Pull Pull, GPIO_Speed Speed) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = Mode;
    GPIO_InitStruct.Speed = Speed;
    GPIO_InitStruct.OutputType = PutType;
    GPIO_InitStruct.Pull =  Pull;

    LL_GPIO_Init(port, &GPIO_InitStruct);
}
