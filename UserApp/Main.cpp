#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_Led_Flash(void);
static void Task_Key(void);

ADS1256 ads1256;


void Init(void)
{
    led.pinkishRed();//初始粉色
    sysTimer.registerCallback(KeyScan10ms).start(100);
    ads1256 = ADS1256(GPIO(GPIOD, 0), GPIO(GPIOD, 1), GPIO(GPIOD, 2), GPIO(GPIOD, 3),
                      GPIO(GPIOD, 4),GPIO(GPIOD, 5),GPIO(GPIOD, 6));
    uint8_t id;
    id = ads1256.readChipID();
    cout << "id = " << id << endl;
    ads1256.config(ADS1256_GAIN_1, ADS1256_15SPS);
    ads1256.start(ADS1256_MODE_Single);

}
void Loop(void)
{
    Task_Led_Flash();
    Task_Key();
    if (ads1256.isFinished())
    {
        cout << ads1256.getVolt(0) << "V" << endl;
    }
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
