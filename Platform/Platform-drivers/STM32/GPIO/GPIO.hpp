#ifndef __PLATFORM_STM32_GPIO_H__
#define __PLATFORM_STM32_GPIO_H__

#include "platform-drivers/STM32/LL-support.h"

namespace STM32
{

    #define PLATFORM_GPIO_PIN_NUM (16U)
    static const uint32_t MapGPIOPin[PLATFORM_GPIO_PIN_NUM] = { LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2, LL_GPIO_PIN_3, LL_GPIO_PIN_4,
                                                             LL_GPIO_PIN_5, LL_GPIO_PIN_6, LL_GPIO_PIN_7, LL_GPIO_PIN_8, LL_GPIO_PIN_9,
                                                            LL_GPIO_PIN_10, LL_GPIO_PIN_11, LL_GPIO_PIN_12, LL_GPIO_PIN_13, LL_GPIO_PIN_14,
                                                            LL_GPIO_PIN_15 };
    /**
     * @brief GPIO模式
     *
     * GPIO的工作模式。
     */
    typedef enum   
    {
        in          =       LL_GPIO_MODE_INPUT ,        /**< 输入模式 */
        out         =       LL_GPIO_MODE_OUTPUT,        /**< 输出模式 */
        analog      =       LL_GPIO_MODE_ANALOG,        /**< 模拟模式 */
        alternate   =       LL_GPIO_MODE_ALTERNATE,     /**< 复用模式 */
    }GPIO_Mode;

    /**
     * @brief GPIO速度
     *
     * GPIO输出速度。
     */
    typedef enum
    {
        slow        =       LL_GPIO_SPEED_FREQ_LOW,      /**< 低速 */
        medium      =       LL_GPIO_SPEED_FREQ_MEDIUM,   /**< 中速 */
        fast        =       LL_GPIO_SPEED_FREQ_HIGH,     /**< 快速 */
        very_fast   =       LL_GPIO_SPEED_FREQ_VERY_HIGH /**< 非常快速 */
    }GPIO_Speed;

    /**
     * @brief GPIO上拉/下拉
     *
     * GPIO上拉或下拉配置。
     */
    typedef enum
    {
        pullno      =       LL_GPIO_PULL_NO,     /**< 不上拉/下拉 */
        pullup      =       LL_GPIO_PULL_UP,     /**< 上拉 */
        pulludown   =       LL_GPIO_PULL_DOWN   /**< 下拉 */
    }GPIO_Pull;

    /**
     * @brief GPIO输出类型
     *
     * GPIO输出类型。
     */
    typedef enum
    {
        pp          =       LL_GPIO_OUTPUT_PUSHPULL,   /**< 推挽输出 */
        od          =       LL_GPIO_OUTPUT_OPENDRAIN    /**< 开漏输出 */
    }GPIO_OutPutType;

    /**
     * @brief GPIO类
     *
     * 用于控制GPIO。
     */
    class GPIO
    {
    public:
        friend class GPIO_EXTI;
    private:
        GPIO_TypeDef* port;
        uint32_t pin;
    public:
        GPIO() = default;
        ~GPIO() = default;
        /**
         * @brief GPIO类构造函数
         *
         * @param Port GPIO端口
         * @param Pin GPIO引脚号
         * @param Pull GPIO上拉/下拉，默认为不上拉/下拉
         * @param Mode GPIO模式，默认为输出模式
         * @param PutType GPIO输出类型，默认为推挽输出
         * @param Speed GPIO输出速度，默认为快速
         */
        GPIO(GPIO_TypeDef* Port, uint8_t Pin, STM32::GPIO_Pull Pull = STM32::pullno, STM32::GPIO_Mode Mode = STM32::out,
             STM32::GPIO_OutPutType PutType = STM32::pp, STM32::GPIO_Speed Speed = STM32::fast);

        /**
         * @brief 将GPIO引脚置为低电平
         */
        inline void low(void);
        /**
         * @brief 将GPIO引脚置为高电平
         */
        inline void high(void);
        /**
         * @brief 翻转GPIO引脚的电平状态
         */
        inline void toggle(void);
        /**
         * @brief 读取GPIO引脚的电平状态
         *
         * @return true GPIO引脚处于高电平状态
         * @return false GPIO引脚处于低电平状态
         */
        inline bool read(void);
        /**
         * @brief 设置GPIO引脚的电平状态
         *
         * @param value true为高电平，false为低电平
         */
        inline void setValue(bool);
        /**
         * @brief 设置GPIO引脚的电平状态
         *
         * @param value 1为高电平，其他值为低电平
         */
        inline GPIO& operator=(int value);
        /**
         * @brief 读取GPIO端口的输入数据寄存器的值
         *
         * @return uint32_t 返回GPIO端口的输入数据寄存器的值
         */
        inline uint32_t readPort(void);
        /**
         * @brief 设置GPIO配置
         *
         * @param Mode GPIO模式
         * @param PutType GPIO输出类型
         * @param Pull GPIO上拉/下拉
         * @param Speed GPIO输出速度
         */
        void setConfig(STM32::GPIO_Pull Pull = STM32::pullno, STM32::GPIO_Mode Mode = STM32::out, STM32::GPIO_OutPutType PutType = STM32::pp, GPIO_Speed Speed = fast);
    };


    inline void GPIO::low(void) {
        WRITE_REG(port->BSRR, pin << 16U);
    }

    inline void GPIO::high(void) {
        WRITE_REG(port->BSRR, pin);
    }

    inline void GPIO::toggle(void) {
        uint32_t odr = READ_REG(port->ODR);
        WRITE_REG(port->BSRR, ((odr & pin) << 16u) | (~odr & pin));
    }

    inline bool GPIO::read(void) {
        bool bitstatus;
        if((port->IDR & pin) != (uint32_t) false)
        bitstatus = true;
        else
        bitstatus = false;
        return bitstatus;
    }

    inline void GPIO::setValue(bool value) {
        if(value)
            high();
        else
            low();
    }

    inline uint32_t GPIO::readPort(void)
    {
        return (uint32_t)(READ_REG(port->IDR));
    }

    GPIO& GPIO::operator=(int value) {
        setValue(value);
        return *this;
    }
}//namespace STM32

#endif //__PLATFORM_STM32_GPIO_H__