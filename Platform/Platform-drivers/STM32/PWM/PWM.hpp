#ifndef __PLATFORM_STM32_PWM_H__
#define __PLATFORM_STM32_PWM_H__

#include "platform-drivers/STM32/Timer/Timer.hpp"

namespace STM32{

/**
 * @brief 定时器通道
 *
 * 定时器通道。
 */
typedef enum {
    channel1    =   LL_TIM_CHANNEL_CH1,  /**< 通道1 */
    channel1N   =   LL_TIM_CHANNEL_CH1N, /**< 通道1N */
    channel2    =   LL_TIM_CHANNEL_CH2,  /**< 通道2 */
    channel2N   =   LL_TIM_CHANNEL_CH2N, /**< 通道2N */
    channel3    =   LL_TIM_CHANNEL_CH3,  /**< 通道3 */
    channel3N   =   LL_TIM_CHANNEL_CH3N, /**< 通道3N */
    channel4    =   LL_TIM_CHANNEL_CH4,  /**< 通道4 */
    channel5    =   LL_TIM_CHANNEL_CH5,  /**< 通道5 */
    channel6    =   LL_TIM_CHANNEL_CH6,  /**< 通道6 */
} Timer_Channel;

/**
 * @brief PWM类
 *
 * 用于控制PWM信号输出。
 */
class PWM : Timer
{
private:
    Timer_Channel channel;
public:
    /* Variables */
    /* Constructor */
    PWM() = default;
    ~PWM() = default;

    /**
     * @brief PWM类构造函数
     *
     * @param TIMx 定时器指针
     * @param Channel 定时器通道
     * @param IsUseIrq 是否使用中断，默认为false
     * @param callback_fun 中断回调函数，默认为nullptr
     */
    PWM(TIM_TypeDef * TIMx, Timer_Channel Channel, bool IsUseIrq = false, Platform_Timer_CallBack_fun callback_fun = nullptr)
                        : Timer(TIMx, IsUseIrq, callback_fun), channel(Channel) { }

    /* Functions */
    /**
     * @brief 启动PWM输出
     *
     * @return PWM& 返回PWM对象的引用
     */
    PWM& start(void);
    /**
     * @brief 启动PWM输出
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @return PWM& 返回PWM对象的引用
     */
    PWM& start(uint32_t timerFreq);
    /**
     * @brief 启动PWM输出
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @param timerCLK 定时器时钟频率（单位：Hz）
     * @return PWM& 返回PWM对象的引用
     */
    PWM& start(uint32_t timerFreq, uint64_t timerCLK);
    /**
    * @brief 停止PWM输出
    *
    * @return PWM& 返回PWM对象的引用
    */
    PWM& stop(void);
    /**
     * @brief 设置PWM频率
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @param timerCLK 定时器时钟频率（单位：Hz）
     * @return PWM& 返回PWM对象的引用
     */
    PWM& setFreq(uint32_t timerFreq, uint64_t timerCLK);
    /**
     * @brief 设置PWM频率
     *
     * @param timerFreq 定时器触发频率（单位：Hz）
     * @return PWM& 返回PWM对象的引用
     */
    PWM& setFreq(uint32_t timerFreq);
};

} //namespace STM32



#endif //__PLATFORM_STM32_PWM_H__
