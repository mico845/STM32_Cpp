//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_PLATFORM_STM32_H
#define CODE_PLATFORM_STM32_H

/* STM32 H7 Includes */
#include "stm32h7xx.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_adc.h"


/* STM32 H7 Memory Address Typedef */
#define     __AT_AXI_SRAM_      __attribute__((section("._AXI_SRAM_Area_512KB")))
#define     __AT_SRAM1_         __attribute__((section("._SRAM1_Area_128KB")))
#define     __AT_SRAM2_         __attribute__((section("._SRAM2_Area_128KB")))
#define     __AT_SRAM3_         __attribute__((section("._SRAM3_Area_32KB")))
#define     __AT_SRAM4_         __attribute__((section("._SRAM4_Area_64KB")))

#define     ALIGN_32B(buf)      buf __attribute__ ((aligned (32)))

/* Namespace */
namespace STM32{
    namespace STM32_GPIO{
        #define PLATFORM_GPIO_PIN_NUM (16U)
        static const uint32_t MapPin[PLATFORM_GPIO_PIN_NUM] = {LL_GPIO_PIN_0,LL_GPIO_PIN_1,LL_GPIO_PIN_2,LL_GPIO_PIN_3,LL_GPIO_PIN_4,
                                                               LL_GPIO_PIN_5,LL_GPIO_PIN_6,LL_GPIO_PIN_7,LL_GPIO_PIN_8,LL_GPIO_PIN_9,
                                                               LL_GPIO_PIN_10,LL_GPIO_PIN_11,LL_GPIO_PIN_12,LL_GPIO_PIN_13,LL_GPIO_PIN_14,
                                                               LL_GPIO_PIN_15};
        typedef enum GPIO_Port  // gpio_port
        {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
        }GPIO_Port;

        typedef enum GPIO_Mode  // gpio_port
        {
            in          =       LL_GPIO_MODE_INPUT ,
            out         =       LL_GPIO_MODE_OUTPUT,
            analog      =       LL_GPIO_MODE_ANALOG,
            alternate   =       LL_GPIO_MODE_ALTERNATE,
        }GPIO_Mode;

        typedef enum GPIO_Speed  // gpio_port
        {
            slow        =       LL_GPIO_SPEED_FREQ_LOW,
            medium      =       LL_GPIO_SPEED_FREQ_MEDIUM,
            fast        =       LL_GPIO_SPEED_FREQ_HIGH,
            very_fast   =       LL_GPIO_SPEED_FREQ_VERY_HIGH,
        }GPIO_Speed;

        typedef enum GPIO_Pull
        {
            pullno      =       LL_GPIO_PULL_NO,
            pullup      =       LL_GPIO_PULL_UP,
            pulludown   =       LL_GPIO_PULL_DOWN,
        }GPIO_Pull;

        typedef enum GPIO_OutPutType
        {
            pp          =       LL_GPIO_OUTPUT_PUSHPULL,
            od          =       LL_GPIO_OUTPUT_OPENDRAIN
        }GPIO_OutPutType;
    }
    namespace STM32_USART{
        #define PLATFORM_USART_NUM (8)
        static USART_TypeDef* const MapUSART[PLATFORM_USART_NUM + 1] = {nullptr, USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8};
    }
    namespace STM32_DMA{
        #define PLATFORM_DMA_NUM (2)
        static DMA_TypeDef* const MapDMA[PLATFORM_DMA_NUM + 1] = {nullptr, DMA1, DMA2};
        #define PLATFORM_DMA_STREAM_NUM (8)
        static const uint32_t MapDMA_Stream[PLATFORM_DMA_STREAM_NUM] = {LL_DMA_STREAM_0, LL_DMA_STREAM_1, LL_DMA_STREAM_2, LL_DMA_STREAM_3,
                                                                        LL_DMA_STREAM_4, LL_DMA_STREAM_5,LL_DMA_STREAM_6,LL_DMA_STREAM_7};
    }
    namespace STM32_EXTI{
        #define PLATFORM_EXTI_LINE_NUM (16)
        static const uint32_t MapDMA_EXTI_LINE[PLATFORM_EXTI_LINE_NUM] = {LL_EXTI_LINE_0,   LL_EXTI_LINE_1,   LL_EXTI_LINE_2,   LL_EXTI_LINE_3,
                                                                          LL_EXTI_LINE_4,   LL_EXTI_LINE_5,   LL_EXTI_LINE_6,   LL_EXTI_LINE_7,
                                                                          LL_EXTI_LINE_8,   LL_EXTI_LINE_9,   LL_EXTI_LINE_10,  LL_EXTI_LINE_11,
                                                                          LL_EXTI_LINE_12,  LL_EXTI_LINE_13,  LL_EXTI_LINE_14,  LL_EXTI_LINE_15
                                                                            };
    }
    namespace STM32_TIM {
    #define PLATFORM_TIM_NUM (17)
        static TIM_TypeDef *const MapTimer[PLATFORM_TIM_NUM + 1] = {nullptr, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, nullptr, nullptr, nullptr, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17};
        typedef enum Timer_Channel
        {
            channel1    =   LL_TIM_CHANNEL_CH1,
            channel1N   =   LL_TIM_CHANNEL_CH1N,
            channel2    =   LL_TIM_CHANNEL_CH2,
            channel2N   =   LL_TIM_CHANNEL_CH2N,
            channel3    =   LL_TIM_CHANNEL_CH3,
            channel3N   =   LL_TIM_CHANNEL_CH3N,
            channel4    =   LL_TIM_CHANNEL_CH4,
            channel5    =   LL_TIM_CHANNEL_CH5,
            channel6    =   LL_TIM_CHANNEL_CH6,
        }Timer_Channel;

    }
    namespace STM32_ADC {
#define PLATFORM_ADC_NUM (3)
        static ADC_TypeDef *const MapADC[PLATFORM_ADC_NUM + 1] = {nullptr, ADC1, ADC2, ADC3};
        typedef enum ADC_CalibrationMode
        {
            offset              =   LL_ADC_CALIB_OFFSET,
            linearity           =   LL_ADC_CALIB_LINEARITY,
            offset_linearity    =   LL_ADC_CALIB_OFFSET_LINEARITY,
        }ADC_CalibrationMode;
        typedef enum ADC_SingleDiff
        {
           single       = LL_ADC_SINGLE_ENDED,
           differential = LL_ADC_DIFFERENTIAL_ENDED,
           botn_single_diff = LL_ADC_BOTH_SINGLE_DIFF_ENDED,
        }ADC_SingleDiff;
    }
}

#endif //CODE_PLATFORM_STM32_H
