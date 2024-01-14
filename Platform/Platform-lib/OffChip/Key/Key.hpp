//
// Created by Whisky on 1/6/2024.
//

#ifndef CODE_KEY_H
#define CODE_KEY_H
#include "OnChip/GPIO/GPIO.hpp"
#include "DataStructre/Fifo.hpp"

//按键数量
#define KEY_HARD_NUM                (1)
//定义KEY1_GPIO
#ifndef KEY1_GPIO_GPIO
    #define KEY1_GPIO_GPIO     (Platform::HardWare::Key::KEY1)  //ID
    #define KEY1_GPIO_PORT     (STM32::STM32_GPIO::C)
    #define KEY1_GPIO_PIN      (13)
    #define KEY1_GPIO_PULL     (STM32::STM32_GPIO::pullup)
#endif

namespace Platform::HardWare::Key{
    /*
    按键滤波时间 50ms, 单位 10ms。
    只有连续检测到 50ms 状态不变才认为有效，包括弹起和按下两种事件
    即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME 5
#define KEY_LONG_TIME 100           /* 单位 10ms， 持续 1 秒，认为长按事件 */

    typedef enum Key_ID{
        KEY1 = 0,
        KEY2,
        KEY3
    }Key_ID;

    typedef enum Key_State
    {
        Key_None = 0,
        Key_Down,
        Key_Up,
        Key_Long,
    }Key_State;

    typedef struct Key_Value
    {
        Key_ID id;
        Key_State state;
    }Key_Value;


}
#define  KEY_ID_STATE(ID, STATE)        ((Platform::HardWare::Key::ID << 4) | (Platform::HardWare::Key::STATE ))

typedef enum Key_TypeDef
{
    Key_None    = 0,
    Key1_Down   = KEY_ID_STATE(KEY1, Key_Down),
    Key1_Up     = KEY_ID_STATE(KEY1, Key_Up),
    Key1_Long   = KEY_ID_STATE(KEY1, Key_Long),

    Key2_Down   = KEY_ID_STATE(KEY2, Key_Down),
    Key2_Up     = KEY_ID_STATE(KEY2, Key_Up),
    Key2_Long   = KEY_ID_STATE(KEY2, Key_Long),

    Key3_Down   = KEY_ID_STATE(KEY3, Key_Down),
    Key3_Up     = KEY_ID_STATE(KEY3, Key_Up),
    Key3_Long   = KEY_ID_STATE(KEY3, Key_Long),

}Key_TypeDef;

class Key
{
public:
    friend void Key_Detect(uint8_t index);
private:
    GPIO key_gpio;
    bool active_level;
    Platform::HardWare::Key::Key_ID id;
    Platform::HardWare::Key::Key_State state;
    uint8_t filter_count;
    uint16_t long_count;
public:
    Key() = default;
    ~Key() = default;
    Key(Platform::HardWare::Key::Key_ID Key_ID, GPIO_Port Port, uint8_t Pin, GPIO_Pull Pull, bool Active_Level = false);
    bool IsActive(void);
    Platform::HardWare::Key::Key_ID Get_ID(void);
};

extern Key _key[KEY_HARD_NUM];
#define KEY_VALUE_FIFO_MAX_SIZE     (10)
extern Fifo<Platform::HardWare::Key::Key_Value, KEY_VALUE_FIFO_MAX_SIZE> Key_Value_Fifo;

void Key_Init(void);
uint8_t Key_Scan(void);
void KeyScan10ms(void);
void Key_Detect(uint8_t index);
Key_TypeDef Key_GetValue(void);

#endif //CODE_KEY_H
