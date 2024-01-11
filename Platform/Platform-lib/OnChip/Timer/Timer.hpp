//
// Created by Whisky on 1/5/2024.
//

#ifndef CODE_TIMER_HPP
#define CODE_TIMER_HPP
#include "Platform-STM32.h"

#ifndef __TYPEDEF_TIMER_CALLBACK__
typedef void (*Platform_Timer_CallBack_fun)(void);
#endif

class Timer
{
protected:
    /* Variables */
    TIM_TypeDef *tim;
    bool isUseIrq;
    Platform_Timer_CallBack_fun callback_fun;
    bool isTimeout;
public:
    /* Variables */
    /* Constructor */
    Timer() = default;
    ~Timer() = default;

    Timer(uint8_t TIMER_ID, bool IsUseIrq = false,Platform_Timer_CallBack_fun callback_fun = nullptr)
    : tim(STM32::STM32_TIM::MapTimer[TIMER_ID]), isUseIrq(IsUseIrq), callback_fun(callback_fun){ }

    /* Functions */
    virtual Timer& Start(void);
    virtual Timer& Start(uint32_t timerFreq);
    virtual Timer& Start(uint32_t timerFreq, uint64_t timerCLK);
    virtual Timer& Stop(void);

    virtual Timer& SetFreq(uint32_t timerFreq);
    virtual Timer& SetFreq(uint32_t timerFreq, uint64_t timerCLK);

    Timer& SetCount(uint32_t Count);

    Timer& RegisterCallback(Platform_Timer_CallBack_fun CallBack_fun);
    Timer& UnRegisterCallback(void);

    uint64_t Get_TimerClock(void);

    bool Is_Timeout(void);

    void Irq(void);
};

#endif //CODE_TIMER_HPP
