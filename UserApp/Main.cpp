//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

using namespace Platform::HardWare;
using namespace STM32;

// 循环中需要执行的任务
static void Task_Key(void);
static void Task_Led_Flash(void);

 void Init(void)
{
    FIR_Init();
    led.PinkishRed();
    timer.RegisterCallback(KeyScan10ms).Start(100);
}

void Loop(void)
{
    Task_Key();
    Task_Led_Flash();
}

static void Task_Key(void)
{
    static Key_TypeDef key_value;
    key_value = Key_GetValue();
    switch (key_value) {
        case Key1_Down:{
            led.Yellow();
        }break;
        case Key1_Long:{
            led.Cyan();
        }break;
        default:
            break;
    }
}

static void Task_Led_Flash(void)
{
    static uint8_t count = 0;
    if (timer.Is_Timeout())
        if (count++ == 50) // 10ms * N
        {
            led.Toggle();
            count = 0;
        }
}
