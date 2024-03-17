#ifndef __PLATFORM_STM32_DELAY_H__
#define __PLATFORM_STM32_DELAY_H__

#include "platform-drivers/STM32/LL-support.h"


namespace STM32{
    /**
     * @brief 初始化延迟函数
     *
     * @param sysclk 单片机主频（单位：MHz）
     */
    void delay_Init(uint16_t sysclk);
    /**
     * @brief 初始化延迟函数，默认使用系统时钟频率
     */
    void delay_Init(void);

    /**
     * @brief 微秒级延迟
     *
     * @param nUs 微秒数
     */
    void delay_us(uint32_t nUs);
    /**
     * @brief 毫秒级延迟
     *
     * @param nMs 毫秒数
     */
    void delay_ms(uint16_t nMs);
} //namespace STM32



#endif // !__PLATFORM_STM32_DELAY_H__