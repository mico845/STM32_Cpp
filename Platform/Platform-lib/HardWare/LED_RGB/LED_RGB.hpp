#ifndef __HARDWARE_LED_RGB_H__
#define __HARDWARE_LED_RGB_H__

#include "platform-drivers/STM32/GPIO/GPIO.hpp"

/**
 * @brief 定义LED_RGB_GPIO
 */
#ifndef LED_RGB_GPIO
    #define LED_RGB_GPIO
    // RED GPIO
    #define LED_RED_PORT        (GPIOC)     /**< RED灯的GPIO端口 */
    #define LED_RED_PIN         (0)         /**< RED灯的GPIO引脚号 */
    // GREEN GPIO
    #define LED_GREEN_PORT      (GPIOC)     /**< GREEN灯的GPIO端口 */
    #define LED_GREEN_PIN       (1)         /**< GREEN灯的GPIO引脚号 */
    // BLUE GPIO
    #define LED_BLUE_PORT       (GPIOC)     /**< BLUE灯的GPIO端口 */
    #define LED_BLUE_PIN        (2)         /**< BLUE灯的GPIO引脚号 */
#endif

/**
 * @brief LED_RGB类
 *
 * RGB灯的驱动。
 */
class LED_RGB
{
private:
    #define __INDEX_LED_R   (0)
    #define __INDEX_LED_G   (1)
    #define __INDEX_LED_B   (2)
    
    bool default_led_value;
    bool current_led_value[3];
    bool toggle_flag = false;
public:
    /* Variables */
    STM32::GPIO Led_R;  /**< 红色LED */
    STM32::GPIO Led_G;  /**< 绿色LED */
    STM32::GPIO Led_B;  /**< 蓝色LED */
    /* Constructor */
    LED_RGB() = default;
    ~LED_RGB() = default;
    /**
     * @brief LED_RGB类构造函数
     * @param LED_R 红色LED的GPIO
     * @param LED_G 绿色LED的GPIO
     * @param LED_B 蓝色LED的GPIO
     * @param led_value LED点亮的默认电平，默认为false，即低电平点亮
     */
    LED_RGB(STM32::GPIO LED_R, STM32::GPIO LED_G, STM32::GPIO LED_B, bool led_value = false)
                    :   default_led_value(led_value),
                        Led_R(LED_R),
                        Led_G(LED_G),
                        Led_B(LED_B)
                    { }
    /* Functions */
    /**
     * @brief 关闭所有LED
     */
    void off(void);
    /**
     * @brief RGB灯切换为红色
     */
    void red(void);
    /**
     * @brief RGB灯切换为绿色
     */
    void green(void);
    /**
     * @brief RGB灯切换为蓝色
     */
    void blue(void);
    /**
     * @brief RGB灯切换为黄色
     */
    void yellow(void);
    /**
     * @brief RGB灯切换为青色
     */
    void cyan(void);
    /**
     * @brief RGB灯切换为粉红色
     */
    void pinkishRed(void);
    /**
     * @brief RGB灯切换为白色
     */
    void white(void);
    /**
     * @brief 翻转RGB灯的电平
     *
     * 关闭全部灯光/切换到关闭之前的灯光颜色
     */
    void toggle(void);
};

#endif //__HARDWARE_LED_RGB_H__
