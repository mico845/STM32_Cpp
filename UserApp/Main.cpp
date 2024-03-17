#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_Led_Flash(void);
static void Task_Key(void);

void Init(void)
{
    led.pinkishRed();//初始粉色
    sysTimer.registerCallback(KeyScan10ms).start(100);
}
void Loop(void)
{
    Task_Led_Flash();
    Task_Key();
}

static void Task_Led_Flash(void)
{
    static uint8_t count = 0;
    if (sysTimer.isTimeOut())
        if (count++ == 50) // 10ms * N
        {
            led.toggle();
            count = 0;
        }
}

static void Task_Key(void)
{
    static Key_TypeDef key_value;
    key_value = Key_GetValue();
    switch (key_value) {
        case Key1_Down:{
            led.yellow();
        }break;
        case Key1_Long:{
            led.cyan();
        }break;
        default:
            break;
    }
}
