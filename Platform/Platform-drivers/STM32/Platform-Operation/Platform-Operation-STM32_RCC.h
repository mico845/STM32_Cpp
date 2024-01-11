//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_Platform_OPERATION_RCC_H
#define CODE_Platform_OPERATION_RCC_H
#include "Platform-STM32.h"

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

static uint32_t Platform_RCC_GetHCLKClockFreq(void)
{
    uint32_t SYSCLK_Frequency = Platform_RCC_GetSystemClockFreq();
    /* HCLK clock frequency */
    return LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, LL_RCC_GetAHBPrescaler());
}

static uint32_t Platform_RCC_GetPCLK1ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK1 clock frequency */
    return LL_RCC_CALC_PCLK1_FREQ(HCLK_Frequency, LL_RCC_GetAPB1Prescaler());
}

static uint32_t Platform_RCC_GetPCLK2ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK2 clock frequency */
    return LL_RCC_CALC_PCLK2_FREQ(HCLK_Frequency, LL_RCC_GetAPB2Prescaler());
}

static uint32_t Platform_RCC_GetPCLK3ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK3 clock frequency */
    return LL_RCC_CALC_PCLK3_FREQ(HCLK_Frequency, LL_RCC_GetAPB3Prescaler());
}

/**
  * @brief  Return PCLK4 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK4 clock frequency (in Hz)
  */
static uint32_t Platform_RCC_GetPCLK4ClockFreq(void)
{
    uint32_t HCLK_Frequency = Platform_RCC_GetHCLKClockFreq();
    /* PCLK4 clock frequency */
    return LL_RCC_CALC_PCLK4_FREQ(HCLK_Frequency, LL_RCC_GetAPB4Prescaler());
}


#endif //CODE_Platform_OPERATION_RCC_H
