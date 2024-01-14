//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

ALIGN_32B(USART           cout  __AT_SRAM1_);
ALIGN_32B(ADC             adc   __AT_SRAM4_);
ALIGN_32B(DAC             dac   __AT_SRAM4_ );
LED_RGB         led     __AT_AXI_SRAM_;
Timer           timer;
DDS             dds;
LCD_SPI         lcd;

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
    dac   = DAC(1, 1, 2, 1, 6);
    dds   = DDS(&dac);
    lcd   = LCD_SPI(true);
}

void HardInit(void)
{
    Key_Init();
    lcd.Clear();
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

