//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_DELAY_H
#define CODE_DELAY_H
#include "Platform-STM32.h"

class Delay
{
private:
    uint32_t fac_us;
    void delay_xms(uint16_t nms);
public:
    Delay();
    ~Delay() = default;
    Delay(uint16_t sysclk);
    void us(uint32_t nUs);
    void ms(uint16_t nMs);
};
#endif //CODE_DELAY_H
