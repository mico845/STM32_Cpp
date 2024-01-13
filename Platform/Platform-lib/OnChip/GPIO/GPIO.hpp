//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_GPIO_H
#define CODE_GPIO_H

#include "Platform-STM32.h"

using namespace STM32::STM32_GPIO;

class GPIO
{
private:
    /* Variables */
    GPIO_Mode mode;
    GPIO_TypeDef* port;
    uint32_t pin;
    /* Functions */
    void RCC_Configuration(void);
public:
    /* Variables */

    /* Constructor */
    GPIO(void) = default;
    ~GPIO(void) = default;

    GPIO(GPIO_Port Port, uint8_t Pin, GPIO_Mode Mode = out, GPIO_OutPutType PutType = pp,
                                    GPIO_Pull Pull = pullno, GPIO_Speed Speed = fast);

    /* Functions */
    __INLINE void Low(void);
    __INLINE void High(void);
    __INLINE void Toggle(void);
    __INLINE bool Read(void);
    __INLINE void SetLeveL(bool Level);
    __INLINE uint32_t ReadPort(void);

    void SetConfig(GPIO_Mode Mode = out, GPIO_OutPutType PutType = pp, GPIO_Pull Pull = pullno, GPIO_Speed Speed = fast);
};

__INLINE void GPIO::Low(void) {
    WRITE_REG(port->BSRR, pin << 16U);
}

__INLINE void GPIO::High(void) {
    WRITE_REG(port->BSRR, pin);
}

__INLINE void GPIO::Toggle(void) {
    uint32_t odr = READ_REG(port->ODR);
    WRITE_REG(port->BSRR, ((odr & pin) << 16u) | (~odr & pin));
}

__INLINE bool GPIO::Read(void) {
    bool bitstatus;
    if((port->IDR & pin) != (uint32_t) false)
    bitstatus = true;
    else
    bitstatus = false;
    return bitstatus;
}

__INLINE void GPIO::SetLeveL(bool Level) {
    if(Level)
        High();
    else
        Low();
}

__INLINE uint32_t GPIO::ReadPort(void)
{
    return (uint32_t)(READ_REG(port->IDR));
}


#endif //CODE_GPIO_H
