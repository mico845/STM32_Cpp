#ifndef __PLATFORM_STM32_GPIO_EXTI_H__
#define __PLATFORM_STM32_GPIO_EXTI_H__

#include "platform-drivers/STM32/GPIO/GPIO.hpp"

namespace STM32{

#ifndef __TYPEDEF_GPIO_EXTI_CALLBACK__
    /**
     * @brief GPIO外部中断回调函数类型定义
     */
    typedef void (*Platform_GPIOEXTI_CallBack_fun)(void);
#endif

/**
 * @brief GPIO外部中断类
 *
 * 用于控制GPIO的外部中断。
 */

class GPIO_EXTI
{
private:
    /* Variables */
    GPIO_TypeDef* port;
    uint32_t pin;
    uint32_t exti_line;
    Platform_GPIOEXTI_CallBack_fun callback_fun;
public:
    /* Variables */

    /* Constructor */
    GPIO_EXTI() = default;
    ~GPIO_EXTI() = default;

    /**
     * @brief GPIO_EXTI类构造函数
     * @param Port GPIO端口
     * @param Pin GPIO引脚号
     * @param CallBack_fun 中断回调函数，默认为nullptr
     */
    GPIO_EXTI(GPIO_TypeDef* Port, uint8_t Pin, Platform_GPIOEXTI_CallBack_fun CallBack_fun = nullptr);
    /* Functions */
    /**
     * @brief 注册中断回调函数
     * @param CallBack_fun 中断回调函数
     * @return GPIO_EXTI& 返回GPIO_EXTI对象的引用
     */
    GPIO_EXTI& registerCallback(Platform_GPIOEXTI_CallBack_fun CallBack_fun);
    /**
     * @brief 注销中断回调函数
     * @return GPIO_EXTI& 返回GPIO_EXTI对象的引用
     */
    GPIO_EXTI& unregisterCallback(void);
    /**
     * @brief 读取GPIO引脚的电平状态
     *
     * @return true GPIO引脚处于高电平状态
     * @return false GPIO引脚处于低电平状态
     */
    bool read(void);
    /**
     * @brief 外部中断服务函数
     */
    void irq(void);
};

} //namespace STM32


#endif //__PLATFORM_STM32_GPIO_EXTI_H__
