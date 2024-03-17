#include "platform-drivers/STM32/PWM/PWM.hpp"
#include "platform-drivers/STM32/RCC/RCC.hpp"

using namespace STM32;

static inline void SetCCRx(TIM_TypeDef *TIMx, STM32::Timer_Channel Channel, uint32_t CompareValue)
{
    if (Channel == channel1 || Channel == channel1N)
        WRITE_REG(TIMx->CCR1, CompareValue);
    else if (Channel == channel2 || Channel == channel2N)
        WRITE_REG(TIMx->CCR2, CompareValue);
    else if (Channel == channel3 || Channel == channel3N)
        WRITE_REG(TIMx->CCR3, CompareValue);
    else if (Channel == channel4)
        WRITE_REG(TIMx->CCR4, CompareValue);
    else if (Channel == channel5)
        WRITE_REG(TIMx->CCR5, CompareValue);
    else if (Channel == channel6)
        WRITE_REG(TIMx->CCR6, CompareValue);
}

PWM &PWM::start(void) {
    LL_TIM_CC_EnableChannel(tim, channel);  //使能输出比较通道
    Timer::start();
    return *this;
}

PWM &PWM::start(uint32_t timerFreq, uint64_t timerCLK) {
    setFreq(timerFreq);
    start();
    return *this;
}

PWM &PWM::start(uint32_t timerFreq) {
    setFreq(timerFreq);
    start();
    return *this;
}

PWM &PWM::stop(void) {
    Timer::stop();
    LL_TIM_CC_DisableChannel(tim, channel);
    return *this;
}

PWM &PWM::setFreq(uint32_t timerFreq, uint64_t timerCLK) {
    Timer::setFreq(timerFreq, timerCLK);
    SetCCRx(tim, channel , tim->ARR / 2);
    return *this;
}

PWM &PWM::setFreq(uint32_t timerFreq) {
    return setFreq(timerFreq, getTimerClock());
}







