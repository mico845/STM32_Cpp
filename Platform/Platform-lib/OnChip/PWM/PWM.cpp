//
// Created by Whisky on 1/10/2024.
//

#include "PWM.hpp"
#include "Platform-Operation/Platform-Operation-STM32_RCC.h"

using namespace STM32::STM32_TIM;

static inline void SetCCRx(TIM_TypeDef *TIM, Timer_Channel Channel, uint32_t CompareValue)
{
    if (Channel == channel1 || Channel == channel1N)
        WRITE_REG(TIM->CCR1, CompareValue);
    else if (Channel == channel2 || Channel == channel2N)
        WRITE_REG(TIM->CCR2, CompareValue);
    else if (Channel == channel3 || Channel == channel3N)
        WRITE_REG(TIM->CCR3, CompareValue);
    else if (Channel == channel4)
        WRITE_REG(TIM->CCR4, CompareValue);
    else if (Channel == channel5)
        WRITE_REG(TIM->CCR5, CompareValue);
    else if (Channel == channel6)
        WRITE_REG(TIM->CCR6, CompareValue);
}

PWM &PWM::Start(void) {
    LL_TIM_CC_EnableChannel(tim, channel);  //使能输出比较通道
    Timer::Start();
    return *this;
}

PWM &PWM::Start(uint32_t timerFreq, uint64_t timerCLK) {
    SetFreq(timerFreq);
    Start();
    return *this;
}

PWM &PWM::Start(uint32_t timerFreq) {
    SetFreq(timerFreq);
    Start();
    return *this;
}

PWM &PWM::Stop(void) {
    Timer::Stop();
    LL_TIM_CC_DisableChannel(tim, channel);
    return *this;
}

PWM &PWM::SetFreq(uint32_t timerFreq, uint64_t timerCLK) {
    Timer::SetFreq(timerFreq, timerCLK);
    SetCCRx(tim, channel , tim->ARR / 2);
    return *this;
}

PWM &PWM::SetFreq(uint32_t timerFreq) {
    return SetFreq(timerFreq, Get_TimerClock());
}







