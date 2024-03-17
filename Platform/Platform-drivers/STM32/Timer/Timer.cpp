#include "platform-drivers/STM32/Timer/Timer.hpp"
#include "platform-drivers/STM32/RCC/RCC.hpp"
#include "platform-drivers/STM32/Timer/Platform_Cal_Timer_ARR_PSC.hpp"
#include "platform-lib/System/callInitFunctions.hpp"

using namespace STM32;

Timer &Timer::start(void) {
    LL_TIM_EnableCounter(tim);  //使能计数
    if (isUseIrq)
        LL_TIM_EnableIT_UPDATE(tim);  //使能更新中断
    LL_TIM_EnableAllOutputs(tim);
    return *this;
}

Timer &Timer::start(uint32_t timerFreq) {
    setFreq(timerFreq);
    start();
    return *this;
}

Timer &Timer::start(uint32_t timerFreq, uint64_t timerCLK) {
    setFreq(timerFreq, timerCLK);
    start();
    return *this;
}

Timer &Timer::stop(void) {
    LL_TIM_DisableCounter(tim);
    if (isUseIrq)
        LL_TIM_DisableIT_UPDATE(tim);  //禁止更新中断
    LL_TIM_DisableAllOutputs(tim);
    return *this;
}

Timer &Timer::setFreq(uint32_t timerFreq, uint64_t timerCLK) {
    uint32_t arr;
    uint16_t psc;
    Platform_Timer_Cal_ARR_PSC(timerCLK, timerFreq, &arr, &psc, Platform_Timer_GetARR_MAX(tim));
    setCount(0);
    WRITE_REG(tim->PSC, psc);
    WRITE_REG(tim->ARR, arr);
    return *this;
}

Timer &Timer::setFreq(uint32_t timerFreq) {
    return setFreq(timerFreq, getTimerClock());
}

Timer &Timer::setCount(uint32_t Count) {
    WRITE_REG(tim->CNT, Count);
    return *this;
}

uint64_t Timer::getTimerClock(void) {
    /* APB1 */
    if( tim == TIM2 || tim == TIM3  || tim == TIM4  || tim == TIM5  ||
        tim == TIM6 || tim == TIM7  ||
        tim == TIM12 || tim == TIM13 || tim == TIM14    )
        return Platform_RCC_GetPCLK1ClockFreq() * 2 ;
    /* APB2 */
    else if (   tim == TIM1 || tim == TIM8  || tim ==  TIM15 || tim == TIM16 ||
                tim == TIM17                                           )
        return Platform_RCC_GetPCLK2ClockFreq() * 2 ;
    return 0;
}

Timer& Timer::registerCallback(Platform_Timer_CallBack_fun CallBack_fun) {
    callback_fun = CallBack_fun;
    return *this;
}

Timer& Timer::unregisterCallback(void) {
    callback_fun = nullptr;
    return *this;
}

void Timer::irq(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(tim) == SET)
    {
        isTimeout = true;
        if (callback_fun != nullptr)
            callback_fun();
        LL_TIM_ClearFlag_UPDATE(tim);
    }
}

bool Timer::isTimeOut(void) {
    if (isTimeout)
    {
        isTimeout = false;
        return true;
    }
    else
        return false;
}

namespace STM32{
    Timer sysTimer;
}

INIT_FUNCTION(SYS_TIM_INIT)
{
    STM32::sysTimer = Timer(SYSTIM_TIMx, true);
}
