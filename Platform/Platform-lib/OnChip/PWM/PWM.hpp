//
// Created by Whisky on 1/10/2024.
//

#ifndef HELLOWORLD_PWM_H
#define HELLOWORLD_PWM_H

#include "OnChip/Timer/Timer.hpp"

class PWM : Timer
{
private:
    STM32::STM32_TIM::Timer_Channel channel;
public:
    /* Variables */
    /* Constructor */
    PWM() = default;
    ~PWM() = default;

    PWM(uint8_t TIMER_ID, STM32::STM32_TIM::Timer_Channel Channel, bool IsUseIrq = false, Platform_Timer_CallBack_fun callback_fun = nullptr)
                        : Timer(TIMER_ID, IsUseIrq, callback_fun), channel(Channel) { }

    /* Functions */
    PWM& Start(void);
    PWM& Start(uint32_t timerFreq);
    PWM& Start(uint32_t timerFreq, uint64_t timerCLK);
    PWM& Stop(void);

    PWM& SetFreq(uint32_t timerFreq, uint64_t timerCLK);
    PWM& SetFreq(uint32_t timerFreq);
};

#endif //HELLOWORLD_PWM_H
