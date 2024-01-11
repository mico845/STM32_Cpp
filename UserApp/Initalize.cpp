//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

ALIGN_32B(__AT_SRAM1_     USART           cout);
ALIGN_32B(__AT_SRAM4_     ADC             adc );
ALIGN_32B(__AT_SRAM4_     DAC             dac );
__AT_AXI_SRAM_             LED_RGB         led;
Timer           timer;


using namespace STM32::STM32_TIM;

void ClassInit(void)
{
    led     =   LED_RGB(GPIO(C, 0, out),
                        GPIO(C, 1, out),
                        GPIO(C, 2, out)
                        );

    cout  = USART(1,
                  1,1,
                  1,0);

    timer = Timer(7, true);

    adc   = ADC(3, 2, 0, 1, channel1);
    dac   = DAC(1, 1, 1, 2, 6);
}

void HardInit(void)
{
    Key_Init();
}

/*  Main Functions  */
void  Main(void)
{
    ClassInit();
    HardInit();
    Init();
    while (true)
    {
        Loop();
    }
}

