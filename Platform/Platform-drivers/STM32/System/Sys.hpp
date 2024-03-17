#ifndef __PLATFORM_STM32_SYS_H__
#define __PLATFORM_STM32_SYS_H__

//#include "cmsis_gcc.h"

namespace STM32
{
    /* Stream */
    /**
     * @brief 换行结构体
     *
     * 用于输出换行符。
     */
    struct ENDL {};
    /**
     * @brief 换行结构体的实例对象，endl
     */
    extern struct ENDL endl;

    /**
     * @brief 清屏结构体
     *
     * 用于执行清屏。
     */
    struct CLEAR {};
    /**
     * @brief 清屏结构体的实例对象，clear
     */
    extern struct CLEAR clear;

}


/* STM32 H7 Memory Address Typedef */
#define     __AT_AXI_SRAM_      __attribute__((section("._AXI_SRAM_Area_512KB")))   /**< 将指定对象存放于AXI_SRAM */
#define     __AT_SRAM1_         __attribute__((section("._SRAM1_Area_128KB")))      /**< 将指定对象存放于SRAM1 */
#define     __AT_SRAM2_         __attribute__((section("._SRAM2_Area_128KB")))      /**< 将指定对象存放于SRAM2 */
#define     __AT_SRAM3_         __attribute__((section("._SRAM3_Area_32KB")))       /**< 将指定对象存放于SRAM3 */
#define     __AT_SRAM4_         __attribute__((section("._SRAM4_Area_64KB")))       /**< 将指定对象存放于SRAM4 */
#define     __AT_CPUFLASH_2to8_ __attribute__((section("._CPU_FLASH")))             /**< 将指定对象存放于内部FLASH */

#define     ALIGN_32B(buf)      buf __attribute__ ((aligned (32)))

/**
 * @brief 使能全局中断
 */
#define ENABLE_INT()	__set_PRIMASK(0)
/**
 * @brief 禁止全局中断
 */
#define DISABLE_INT()	__set_PRIMASK(1)

#endif // !__PLATFORM_STM32_SYS_H__