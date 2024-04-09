#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_Led_Flash(void);
static void Task_Key(void);

AD9959 ad9959;

void Init(void)
{
    led.pinkishRed();//初始粉色
    sysTimer.registerCallback(KeyScan10ms).start(100);

    ad9959 = AD9959(GPIO(GPIOD, 0), GPIO(GPIOD, 1), GPIO(GPIOD, 2), GPIO(GPIOD, 3),
           GPIO(GPIOD, 4), GPIO(GPIOD, 5), GPIO(GPIOD, 6), GPIO(GPIOD, 7),
           GPIO(GPIOB, 3), GPIO(GPIOB, 4), GPIO(GPIOB, 5), GPIO(GPIOB, 6),
           GPIO(GPIOB, 7));
    ad9959.setPhaseSweep(Channel0,					0, 		16383,			1,							1,				250,		250,	10000,	1023).update();
}

void Loop(void)
{
    ad9959.update_SK(0b1111);
    delay_ms(33);
    ad9959.update_SK(0);
    delay_ms(33);
 //   delay_ms(2);
 //   Task_Led_Flash();
 //   Task_Key();
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
