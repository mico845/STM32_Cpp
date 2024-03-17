#ifndef __PLATFORM_STM32_RCC_H__
#define __PLATFORM_STM32_RCC_H__

#include "platform-drivers/STM32/LL-support.h"

/**
 * @brief 获取系统时钟频率
 *
 * 根据当前系统时钟源获取系统时钟频率。
 * @return 系统时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetSystemClockFreq(void)
{
    uint32_t frequency = 0U;
    LL_PLL_ClocksTypeDef PLL_Clocks;

    /* Get SYSCLK source -------------------------------------------------------*/
    switch (LL_RCC_GetSysClkSource())
    {
        /* No check on Ready: Won't be selected by hardware if not */
        case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:
            frequency = HSI_VALUE >> (LL_RCC_HSI_GetDivider()>> RCC_CR_HSIDIV_Pos);
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_CSI:
            frequency = CSI_VALUE;
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:
            frequency = HSE_VALUE;
            break;

        case LL_RCC_SYS_CLKSOURCE_STATUS_PLL1:
            LL_RCC_GetPLL1ClockFreq(&PLL_Clocks);
            frequency = PLL_Clocks.PLL_P_Frequency;
            break;

        default:
            /* Nothing to do */
            break;
    }

    return frequency;
}

/**
 * @brief 获取HCLK时钟频率
 *
 * 获取主时钟(HCLK)的频率。
 * @return HCLK时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetHCLKClockFreq(void)
{
    uint32_t SYSCLK_Frequency = Platform_RCC_GetSystemClockFreq();
    /* HCLK clock frequency */
    return LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, LL_RCC_GetAHBPrescaler());
}

/**
 * @brief 获取PCLK1时钟频率
 *
 * 获取外设时钟(PCLK1)的频率。
 * @return PCLK1时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetPCLK1ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK1 clock frequency */
    return LL_RCC_CALC_PCLK1_FREQ(HCLK_Frequency, LL_RCC_GetAPB1Prescaler());
}

/**
 * @brief 获取PCLK2时钟频率
 *
 * 获取外设时钟(PCLK2)的频率。
 * @return PCLK2时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetPCLK2ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK2 clock frequency */
    return LL_RCC_CALC_PCLK2_FREQ(HCLK_Frequency, LL_RCC_GetAPB2Prescaler());
}

/**
 * @brief 获取PCLK3时钟频率
 *
 * 获取外设时钟(PCLK3)的频率。
 * @return PCLK3时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetPCLK3ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK3 clock frequency */
    return LL_RCC_CALC_PCLK3_FREQ(HCLK_Frequency, LL_RCC_GetAPB3Prescaler());
}

/**
 * @brief 获取PCLK4时钟频率
 *
 * 获取外设时钟(PCLK4)的频率。
 * @return PCLK4时钟频率（单位：Hz）
 */
static uint32_t Platform_RCC_GetPCLK4ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK4 clock frequency */
    return LL_RCC_CALC_PCLK4_FREQ(HCLK_Frequency, LL_RCC_GetAPB4Prescaler());
}

#endif // !__PLATFORM_STM32_RCC_H__