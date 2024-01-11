//
// Created by Whisky on 1/6/2024.
//

#include "Key.hpp"
#include "Machine/Platform-Machine.hpp"
using namespace Platform::HardWare::Key;
using namespace Platform::Machine;

Fifo<Platform::HardWare::Key::Key_Value, KEY_VALUE_FIFO_MAX_SIZE> Key_Value_Fifo;
Key _key[KEY_HARD_NUM];

void Key_Detect(uint8_t index);

Key::Key(Key_ID Key_ID, GPIO_Port Port, uint8_t Pin, GPIO_Pull Pull, bool Active_Level)
                                : id(Key_ID), active_level(Active_Level)
{
    key_gpio = GPIO(Port, Pin, in, pp, Pull, very_fast);
    long_count = KEY_LONG_TIME;
    filter_count = KEY_FILTER_TIME / 2;
    state = Platform::HardWare::Key::Key_None;
}

bool Key::IsActive(void) {
    if (key_gpio.Read() == active_level)
        return true;
    else
        return false;
}

Key_ID Key::Get_ID(void) {
    return id;
}

void Key_Init(void)
{
    _key[0] = Key(KEY1, C, 13, pullup);
}

uint8_t Key_Scan(void)
{
    for (int i = 0; i < KEY_HARD_NUM; ++i) {
        if (_key[i].IsActive())
        {
            delay.ms(100);
            if (_key[i].IsActive())
                return _key[i].Get_ID();
        }

    }
    return UINT8_MAX;
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
    if (pBtn->IsActive())
    {
        if (pBtn->filter_count < KEY_FILTER_TIME)
            pBtn->filter_count = KEY_FILTER_TIME;
        else if(pBtn->filter_count < 2 * KEY_FILTER_TIME)
            pBtn->filter_count++;
        else
        {
            if (pBtn->state == Platform::HardWare::Key::Key_None)
            {
                pBtn->state = Key_Down;
                btnValue.id = pBtn->id;
                btnValue.state = pBtn->state;
                Key_Value_Fifo.Put(btnValue);
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
                        Key_Value_Fifo.Put(btnValue);
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
            if (pBtn->state != Platform::HardWare::Key::Key_None)
            {
                pBtn->state = Key_Up;
                btnValue.id = pBtn->id;
                btnValue.state = pBtn->state;
                Key_Value_Fifo.Put(btnValue);
                /* 按键弹起后清空键值 */
                pBtn->state = Platform::HardWare::Key::Key_None;
            }
        }
        pBtn->long_count = 0;
    }
}

Key_TypeDef TypeConverse_KeyValue(Platform::HardWare::Key::Key_Value value)
{
    return (Key_TypeDef)((value.id) << 4 | (value.state));
}

Key_TypeDef Key_GetValue(void)
{
    if (Key_Value_Fifo.Size() == 0)
        return Key_TypeDef::Key_None;

    else
    {
        Key_Value value;
        Key_Value_Fifo.Get(value);
        return TypeConverse_KeyValue(value);
    }

}

