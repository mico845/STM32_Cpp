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
    void Low(void);
    void High(void);
    void Toggle(void);
    bool Read(void);
    void SetLeveL(bool Level);
    __INLINE uint32_t ReadPort(void)
    {
        return (uint32_t)(READ_REG(port->IDR));
    }

    void SetConfig(GPIO_Mode Mode = out, GPIO_OutPutType PutType = pp, GPIO_Pull Pull = pullno, GPIO_Speed Speed = fast);
};


#endif //CODE_GPIO_H
