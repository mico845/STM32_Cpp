#ifndef __PLATFORM_STM32_TIMER_H__
#define __PLATFORM_STM32_TIMER_H__

#include "platform-drivers/STM32/LL-support.h"

#ifndef SYSTIM_TIMx
    #define SYSTIM_TIM_ID               7                /**< Platform库系统定时器使用的定时器号 */
    #define _SYSTIM_TIM_LINK(AA,BB)     AA##BB
    #define __SYSTIM_TIM_LINK(AA,BB)    _SYSTIM_TIM_LINK(AA,BB)
    #define SYSTIM_TIMx                 __SYSTIM_TIM_LINK(TIM, SYSTIM_TIM_ID)
#endif // !SYSTIM_TIMx

namespace STM32{

#ifndef __TYPEDEF_TIMER_CALLBACK__
    /**
     * @brief 定时器回调函数类型
     */
    typedef void (*Platform_Timer_CallBack_fun)(void);
#endif


/**
 * @brief Timer类
 *
 * 用于控制定时器。
 */
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
    /**
     * @brief Timer类构造函数
     *
     * @param TIM 定时器指针
     * @param IsUseIrq 是否使用中断，默认为false
     * @param callback_fun 定时器回调函数，默认为nullptr
     */
    Timer(TIM_TypeDef *TIM, bool IsUseIrq = false, Platform_Timer_CallBack_fun callback_fun = nullptr)
    : tim(TIM), isUseIrq(IsUseIrq), callback_fun(callback_fun) { }

    /* Functions */
    /**
     * @brief 启动定时器
     *
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& start(void);
    /**
     * @brief 启动定时器
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& start(uint32_t timerFreq);
    /**
     * @brief 启动定时器
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @param timerCLK 定时器时钟频率（单位：Hz）
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& start(uint32_t timerFreq, uint64_t timerCLK);
    /**
     * @brief 停止定时器
     *
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& stop(void);

    /**
     * @brief 设置定时器频率
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& setFreq(uint32_t timerFreq);
    /**
     * @brief 设置定时器频率
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @param timerCLK 定时器时钟频率（单位：Hz）
     * @return Timer& 返回Timer对象的引用
     */
    virtual Timer& setFreq(uint32_t timerFreq, uint64_t timerCLK);

    /**
     * @brief 设置定时器计数
     *
     * @param Count 定时器计数值
     * @return Timer& 返回Timer对象的引用
     */
    Timer& setCount(uint32_t Count);

    /**
     * @brief 注册定时器回调函数
     *
     * @param CallBack_fun 回调函数指针
     * @return Timer& 返回Timer对象的引用
     */
    Timer& registerCallback(Platform_Timer_CallBack_fun CallBack_fun);

    /**
     * @brief 注销注册定时器回调函数
     *
     * @return Timer& 返回Timer对象的引用
     */
    Timer& unregisterCallback(void);

    /**
     * @brief 获取定时器时钟频率
     *
     * @return uint64_t 定时器时钟频率（单位：Hz）
     */
    uint64_t getTimerClock(void);

    /**
     * @brief 检查定时器是否超时
     *
     * 必须使用中断才能获取标志位
     *
     * @return true 已经达到超时
     * @return false 尚未达到超时
     */
    bool isTimeOut(void);

    /**
     * @brief 定时器中断服务函数
     */
    void irq(void);
};

/**
 * @brief Platform库系统定时器
 *
 * 用于执行系统任务调度。
 *
 * 如需更换其他定时器，可修改宏SYSTIM_TIM_ID进行切换定时器号TIMx。
 *
 */
extern Timer sysTimer;

} //namespace STM32

#endif //__PLATFORM_STM32_TIMER_H__
