#ifndef __HARDWARE_ADS1256_H__
#define __HARDWARE_ADS1256_H__

#include "platform-drivers/STM32/EXTI/GPIO_EXTI.hpp"


#define ADS1256_VREF        (2509700)
/* 4194303 = 2509700uV , 这是理论值，实际可以根据2.5V基准的实际值进行公式矫正 */
#define ADS1256_VREF_Encode (4194303)

/* 增益选项 */
typedef enum
{
    ADS1256_GAIN_1			= (0),	/* 增益1（缺省） */
    ADS1256_GAIN_2			= (1),	/* 增益2 */
    ADS1256_GAIN_4			= (2),	/* 增益4 */
    ADS1256_GAIN_8			= (3),	/* 增益8 */
    ADS1256_GAIN_16			= (4),	/* 增益16 */
    ADS1256_GAIN_32			= (5),	/* 增益32 */
    ADS1256_GAIN_64			= (6),	/* 增益64 */
}ADS1256_GAIN;

/* 采样速率选项 */
/* 数据转换率选择
	11110000 = 30,000SPS (default)
	11100000 = 15,000SPS
	11010000 = 7,500SPS
	11000000 = 3,750SPS
	10110000 = 2,000SPS
	10100001 = 1,000SPS
	10010010 = 500SPS
	10000010 = 100SPS
	01110010 = 60SPS
	01100011 = 50SPS
	01010011 = 30SPS
	01000011 = 25SPS
	00110011 = 15SPS
	00100011 = 10SPS
	00010011 = 5SPS
	00000011 = 2.5SPS
*/
typedef enum
{
    ADS1256_30000SPS = 0,
    ADS1256_15000SPS,
    ADS1256_7500SPS,
    ADS1256_3750SPS,
    ADS1256_2000SPS,
    ADS1256_1000SPS,
    ADS1256_500SPS,
    ADS1256_100SPS,
    ADS1256_60SPS,
    ADS1256_50SPS,
    ADS1256_30SPS,
    ADS1256_25SPS,
    ADS1256_15SPS,
    ADS1256_10SPS,
    ADS1256_5SPS,
    ADS1256_2d5SPS,

    ADS1256_DRATE_MAX
}ADS1256_DRATE;

#define ADS1256_DRAE_COUNT = 15;

typedef enum
{
    ADS1256_MODE_Single,
    ADS1256_MODE_Differential
}ADS1256_MODE;

class ADS1256
{
private:
    STM32::GPIO sck_gpio;
    STM32::GPIO din_gpio;
    STM32::GPIO cs_gpio;
    STM32::GPIO_EXTI drdy_gpio;
    STM32::GPIO dout_gpio;
    STM32::GPIO sync_gpio;
    STM32::GPIO rst_gpio;

    STM32::Platform_GPIOEXTI_CallBack_fun callback_fun;

    uint8_t Gain;		/* 增益 */
    int32_t AdcNow[8] = {0};			/* 8路ADC采集结果（实时）有符号数 */
    uint8_t Channel = 0;			/* 当前通道 */
    bool flag_finished = false;
    ADS1256_MODE Mode;

    void waitDRDY(void);
    void send8Bit(uint8_t _data);
    uint8_t recive8Bit(void);
    void resetHard(void);
    void delaySCLK(void);
    void delayDATA(void);

    void writeCmd(uint8_t _cmd);
    void writeReg(uint8_t _RegID, uint8_t _RegValue);
    uint8_t readReg(uint8_t _RegID);
    int32_t readData(void);
    void setChannal(uint8_t _ch);
    void setDiffChannal(uint8_t _ch);
public:
    ADS1256() = default;
    ~ADS1256() = default;

    ADS1256(STM32::GPIO SCK_GPIO, STM32::GPIO DIN_GPIO, STM32::GPIO CS_GPIO, STM32::GPIO DRDY_GPIO, STM32::GPIO DOUT_GPIO,
            STM32::GPIO SYNC_GPIO, STM32::GPIO RST_GPIO, STM32::Platform_GPIOEXTI_CallBack_fun Callback = nullptr);

    uint8_t readChipID(void);
    ADS1256& config(ADS1256_GAIN _gain, ADS1256_DRATE _drate);

    int32_t readADC(uint8_t _ch); /* 直接读取，不通过中断 */

    int32_t getData(uint8_t _ch);
    double getVolt(uint8_t _ch);
    void start(ADS1256_MODE mode = ADS1256_MODE_Differential);
    void stop(void);
    bool isFinished(void);
    void irq(void);
    uint8_t getGain(void);
};

#endif //__HARDWARE_ADS1256_H__
