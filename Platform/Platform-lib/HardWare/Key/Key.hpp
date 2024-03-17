#ifndef __HARDWARE_KEY_H__
#define __HARDWARE_KEY_H__

#include "platform-drivers/STM32/GPIO/GPIO.hpp"
#include "platform-lib/DataStructre/Fifo.hpp"

/**
 * @brief Key的编号
 *
 * 按钮的编号。
 */
typedef enum {
    KEY1 = 0,   /**< 1号按键 */
    KEY2,       /**< 2号按键 */
    KEY3,       /**< 3号按键 */
    NO_KEY = UINT8_MAX  /**< 无按键 */
}Key_ID;

#define KEY_HARD_NUM                (1)     /**< 按钮的数量 */

/**
 * @brief 定义KEY1_GPIO
 */
#ifndef KEY1_GPIO_GPIO
    #define KEY1_GPIO_GPIO     (KEY1)   /**< KEY1的编号 */
    #define KEY1_GPIO_PORT     (GPIOC)  /**< KEY1的GPIO端口 */
    #define KEY1_GPIO_PIN      (13)     /**< KEY1的GPIO引脚号 */
    #define KEY1_GPIO_PULL     (STM32::pullup)  /**< KEY1的上/下拉 */
#endif


/**
 * @brief 按键滤波时间
 *
 *  按键滤波时间 50ms, 单位 10ms。
    只有连续检测到 50ms 状态不变才认为有效，包括弹起和按下两种事件
    即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
 */
#define KEY_FILTER_TIME 5
/**
 * @brief 按键长按时间
 *
 *  单位 10ms， 持续 1 秒，认为长按事件
 */
#define KEY_LONG_TIME 100

/**
 * @brief Key的状态
 *
 *  按键的状态。
 */
typedef enum 
{
    Key_Nothing = 0,        /**< 无键操作 */
    Key_Down,               /**< 按键按下 */
    Key_Up,                 /**< 按键释放 */
    Key_Long,               /**< 长按 */
}Key_State;


typedef struct
{
    Key_ID id;
    Key_State state;
}Key_Value;

#define  KEY_ID_STATE(ID, STATE)        ((ID << 4) | (STATE))

/**
 * @brief 各个按键状态
 */
typedef enum
{
    Key_None    = 0, /**< 无按键状态 */

    /** @name 按键1状态 */
    /** @{ */
    Key1_Down   = KEY_ID_STATE(KEY1, Key_Down), /**< 按键1按下状态 */
    Key1_Up     = KEY_ID_STATE(KEY1, Key_Up), /**< 按键1释放状态 */
    Key1_Long   = KEY_ID_STATE(KEY1, Key_Long), /**< 按键1长按状态 */
    /** @} */

    /** @name 按键2状态 */
    /** @{ */
    Key2_Down   = KEY_ID_STATE(KEY2, Key_Down), /**< 按键2按下状态 */
    Key2_Up     = KEY_ID_STATE(KEY2, Key_Up), /**< 按键2释放状态 */
    Key2_Long   = KEY_ID_STATE(KEY2, Key_Long), /**< 按键2长按状态 */
    /** @} */

    /** @name 按键3状态 */
    /** @{ */
    Key3_Down   = KEY_ID_STATE(KEY3, Key_Down), /**< 按键3按下状态 */
    Key3_Up     = KEY_ID_STATE(KEY3, Key_Up), /**< 按键3释放状态 */
    Key3_Long   = KEY_ID_STATE(KEY3, Key_Long), /**< 按键3长按状态 */
    /** @} */

}Key_TypeDef;

/**
 * @brief Key类
 *
 * 按键的驱动。
 */
class Key
{
public:
    friend void Key_Detect(uint8_t index);
private:
    STM32::GPIO key_gpio;
    bool active_level;
    Key_ID id;
    Key_State state;
    uint8_t filter_count;
    uint16_t long_count;
public:
    Key() = default;
    ~Key() = default;
    /**
     * @brief Key类构造函数
     * @param Key_ID Key的编号
     * @param Port GPIO端口
     * @param Pin GPIO引脚号
     * @param Pull GPIO上拉/下拉设置
     * @param Active_Level 按键激活电平，默认为false，即低电平触发按键
     */
    Key(Key_ID Key_ID, GPIO_TypeDef *Port, uint8_t Pin, STM32::GPIO_Pull Pull, bool Active_Level = false);
    /**
     * @brief 检查按键是否处于活动状态
     * @return true 按键此时按下
     * @return false 按键此时未按下
     */
    bool isActive(void);
    /**
     * @brief 获取按键编号
     * @return Key_ID 按键编号
     */
    Key_ID getID(void);
};

extern Key _key[KEY_HARD_NUM];
#define KEY_VALUE_FIFO_MAX_SIZE     (10)                /**< 按键FIFO缓冲区最大大小 */
extern Fifo<Key_Value, KEY_VALUE_FIFO_MAX_SIZE> Key_Value_Fifo;

/**
 * @brief 初始化每个按键
 */
void Key_Init(void);

/**
 * @brief 按键扫描
 * @return Key_ID 被触发按键的编号
 */
Key_ID Key_Scan(void);

/**
 * @brief 10毫秒按键扫描
 *
 * 放在10ms触发的任务中。
 */
void KeyScan10ms(void);
/**
 * @brief 单个按键检测函数
 * @param index 按键索引
 */
void Key_Detect(uint8_t index);

/**
 * @brief 获取按键值
 * @return Key_TypeDef 从按键FIFO缓冲区中读取被按下的值
 */
Key_TypeDef Key_GetValue(void);

#endif //!__HARDWARE_KEY_H__
