#include "platform-lib/HardWare/Key/Key.hpp"
#include "platform-drivers/STM32/Delay/Delay.hpp"
#include "platform-lib/System/callInitFunctions.hpp"
#include "platform-drivers/STM32/Timer/Timer.hpp"

using namespace STM32;

Key _key[KEY_HARD_NUM];
Fifo<Key_Value, KEY_VALUE_FIFO_MAX_SIZE> Key_Value_Fifo;

void Key_Detect(uint8_t index);

Key::Key(Key_ID Key_ID, GPIO_TypeDef *Port, uint8_t Pin, GPIO_Pull Pull, bool Active_Level)
                                : id(Key_ID), active_level(Active_Level)
{
    key_gpio = GPIO(Port, Pin, in, pp, Pull, very_fast);
    long_count = KEY_LONG_TIME;
    filter_count = KEY_FILTER_TIME / 2;
    state = Key_Nothing;
}

bool Key::isActive(void) {
    if (key_gpio.read() == active_level)
        return true;
    else
        return false;
}

Key_ID Key::getID(void) {
    return id;
}

Key_ID Key_Scan(void)
{
    for (int i = 0; i < KEY_HARD_NUM; ++i) {
        if (_key[i].isActive())
        {
            delay_ms(100);
            if (_key[i].isActive())
                return _key[i].getID();
        }

    }
    return NO_KEY;
}

void KeyScan10ms(void)
{
    uint8_t i;
    for (i = 0; i < KEY_HARD_NUM; i++)
        Key_Detect(i);

}

void Key_Detect(uint8_t index)
{
    Key *pBtn;
    Key_Value btnValue;
    pBtn = &_key[index];
    if (pBtn->isActive())
    {
        if (pBtn->filter_count < KEY_FILTER_TIME)
            pBtn->filter_count = KEY_FILTER_TIME;
        else if(pBtn->filter_count < 2 * KEY_FILTER_TIME)
            pBtn->filter_count++;
        else
        {
            if (pBtn->state == Key_Nothing)
            {
                pBtn->state = Key_Down;
                btnValue.id = pBtn->id;
                btnValue.state = pBtn->state;
                Key_Value_Fifo.put(btnValue);
            }
#if (KEY_LONG_TIME > 0)
            {
                if (pBtn->long_count < KEY_LONG_TIME)
                {
                    if (++pBtn->long_count == KEY_LONG_TIME)
                    {
                        pBtn->state = Key_Long;
                        btnValue.id = pBtn->id;
                        btnValue.state = pBtn->state;
                        Key_Value_Fifo.put(btnValue);
                    }
                }
            }
#endif
        }
    }
    else
    {
        if(pBtn->filter_count > KEY_FILTER_TIME)
        {
            pBtn->filter_count = KEY_FILTER_TIME;
        }
        else if(pBtn->filter_count != 0)
        {
            pBtn->filter_count--;
        }
        else
        {
            if (pBtn->state != Key_Nothing)
            {
                pBtn->state = Key_Up;
                btnValue.id = pBtn->id;
                btnValue.state = pBtn->state;
                Key_Value_Fifo.put(btnValue);
                /* 按键弹起后清空键值 */
                pBtn->state = Key_Nothing;
            }
        }
        pBtn->long_count = 0;
    }
}

Key_TypeDef TypeConverse_KeyValue(Key_Value value)
{
    return (Key_TypeDef)((value.id) << 4 | (value.state));
}

Key_TypeDef Key_GetValue(void)
{
    if (Key_Value_Fifo.size() == 0)
        return Key_TypeDef::Key_None;

    else
    {
        Key_Value value;
        Key_Value_Fifo.get(value);
        return TypeConverse_KeyValue(value);
    }

}

void Key_Init(void)
{
    _key[0] = Key(KEY1_GPIO_GPIO, KEY1_GPIO_PORT, KEY1_GPIO_PIN, KEY1_GPIO_PULL);
}

INIT_FUNCTION(KEY_INIT)
{
    Key_Init();
}
