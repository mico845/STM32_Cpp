#include "platform-drivers/STM32/GPIO/GPIO.hpp"

using namespace STM32;

GPIO::GPIO(GPIO_TypeDef* Port, uint8_t Pin, GPIO_Mode Mode, GPIO_OutPutType PutType, GPIO_Pull Pull, GPIO_Speed Speed)
                : port(Port), pin(MapGPIOPin[Pin])
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(Port==GPIOA)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    else if(Port==GPIOB)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
    else if(Port==GPIOC)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    else if(Port==GPIOD)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
    else if(Port==GPIOE)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
    else if(Port==GPIOF)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
    else if(Port==GPIOG)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
    else if(Port==GPIOH)
        LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOH);

    WRITE_REG(Port->BSRR, MapGPIOPin[Pin] << 16U);

    GPIO_InitStruct.Pin = MapGPIOPin[Pin];
    GPIO_InitStruct.Mode = Mode;
    GPIO_InitStruct.Speed = Speed;
    GPIO_InitStruct.OutputType = PutType;
    GPIO_InitStruct.Pull =  Pull;

    LL_GPIO_Init(Port, &GPIO_InitStruct);
}


void GPIO::setConfig(GPIO_Mode Mode, GPIO_OutPutType PutType, GPIO_Pull Pull, GPIO_Speed Speed) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = Mode;
    GPIO_InitStruct.Speed = Speed;
    GPIO_InitStruct.OutputType = PutType;
    GPIO_InitStruct.Pull =  Pull;

    LL_GPIO_Init(port, &GPIO_InitStruct);
}