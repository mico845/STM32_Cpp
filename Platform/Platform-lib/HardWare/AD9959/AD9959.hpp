#ifndef __HARDWARE_AD9959_H__
#define __HARDWARE_AD9959_H__

#include "platform-drivers/STM32/GPIO/GPIO.hpp"

typedef enum AD9959_Channel
{
    Channel0 = 0x10,
    Channel1 = 0x20,
    Channel2 = 0x40,
    Channel3 = 0x80,
}AD9959_Channel;     //CSR[7:4]通道选择启用位

class AD9959
{
private:
    STM32::GPIO cs_gpio;
    STM32::GPIO sclk_gpio;
    STM32::GPIO update_gpio;
    STM32::GPIO ps0_gpio;
    STM32::GPIO ps1_gpio;
    STM32::GPIO ps2_gpio;
    STM32::GPIO ps3_gpio;
    STM32::GPIO sdio0_gpio;
    STM32::GPIO sdio1_gpio;
    STM32::GPIO sdio2_gpio;
    STM32::GPIO sdio3_gpio;
    STM32::GPIO pwr_gpio;
    STM32::GPIO reset_gpio;

    void intserve(void);
    void intReset(void);

    /* AD9959基本寄存器操作函数*/
    void writeData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);
    void write_CFTW0(uint32_t fre);								//写CFTW0通道频率转换字寄存器
    void write_ACR(uint16_t Ampli);								//写ACR通道幅度转换字寄存器
    void write_CPOW0(uint16_t Phase);							//写CPOW0通道相位转换字寄存器

    void write_LSRR(uint8_t rsrr, uint8_t fsrr);				 //写LSRR线性扫描斜率寄存器
    void write_RDW(uint32_t r_delta);							//写RDW上升增量寄存器
    void write_FDW(uint32_t f_delta);							//写FDW下降增量寄存器

    void write_Profile_Fre(uint8_t profile, uint32_t data);      //写Profile寄存器,频率
    void write_Profile_Ampli(uint8_t profile, uint16_t data);    //写Profile寄存器,幅度
    void write_Profile_Phase(uint8_t profile, uint16_t data);    //写Profile寄存器,相位

    void SetModulation(AD9959_Channel Channel, uint8_t Modulation, uint8_t Sweep_en, uint8_t Nlevel);

public:
    AD9959() = default;
    ~AD9959() = default;
    /**
     * @brief AD9959类构造函数
     *
     * @param CS_GPIO AD9959的CS引脚的GPIO
     * @param SCLK_GPIO AD9959的SCLK引脚的GPIO
     * @param UPDATE_GPIO AD9959的UPDATE引脚的GPIO
     * @param PS0_GPIO AD9959的PS0引脚的GPIO
     * @param PS1_GPIO AD9959的PS1引脚的GPIO
     * @param PS2_GPIO AD9959的PS2引脚的GPIO
     * @param PS3_GPIO AD9959的PS3引脚的GPIO
     * @param SDIO0_GPIO AD9959的SDIO0引脚的GPIO
     * @param SDIO1_GPIO AD9959的SDIO1引脚的GPIO
     * @param SDIO2_GPIO AD9959的SDIO2引脚的GPIO
     * @param SDIO3_GPIO AD9959的SDIO3引脚的GPIO
     * @param PWR_GPIO AD9959的PWR引脚的GPIO
     * @param RESET_GPIO AD9959的RESET引脚的GPIO
     */
    AD9959(STM32::GPIO CS_GPIO, STM32::GPIO SCLK_GPIO, STM32::GPIO UPDATE_GPIO, STM32::GPIO PS0_GPIO,
           STM32::GPIO PS1_GPIO, STM32::GPIO PS2_GPIO, STM32::GPIO PS3_GPIO, STM32::GPIO SDIO0_GPIO,
           STM32::GPIO SDIO1_GPIO, STM32::GPIO SDIO2_GPIO, STM32::GPIO SDIO3_GPIO, STM32::GPIO PWR_GPIO,
           STM32::GPIO RESET_GPIO);
    /**
     * @brief 设定AD9959通道频率，范围0~200 000 000 Hz
     *
     * @param Channel   通道对应值
     * @param Freq      输出频率， 范围0~200 000 000 Hz
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setFre(AD9959_Channel Channel,uint32_t Freq);
    /**
     * @brief 设定AD9959通道幅值，控制值0~1023对应输出幅度0~500mVpp左右
     *
     * @param Channel   通道对应值
     * @param Ampli     输出幅度，范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setAmp(AD9959_Channel Channel, uint16_t Ampli);
    /**
     * @brief 设定AD9959通道相位，控制值0~16383对应输出角度0°~360°
     *
     * @param Channel   通道对应值
     * @param Ampli     输出相位, 范围：0~16383(对应角度：0°~360°)
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setPhase(AD9959_Channel Channel,uint16_t Phase);
    /**
     * @brief 更新数据，仅执行该函数，AD9959设定数据生效
     *
     */
    void update(void);
    /**
     * @brief 设置FSK调制
     *
     * @param Channel   通道对应值
     * @param Nlevel    调制阶数，对应有2、4、8、16可填。
     * @param data      数据缓存区
     * @param Phase     输出相位,范围：0~16383(对应角度：0°~360°)
     *
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setFSK(AD9959_Channel Channel, uint8_t Nlevel, uint32_t *data, uint16_t Phase);
    /**
     * @brief 设置ASK调制
     *
     * @param Channel   通道对应值
     * @param Nlevel    调制阶数，对应有2、4、8、16可填。
     * @param data      数据缓存区
     * @param fre       输出频率，范围0~200 000 000 Hz
     * @param Phase     输出相位,范围：0~16383(对应角度：0°~360°)
     *
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setASK(AD9959_Channel Channel, uint8_t Nlevel, uint16_t *data,uint32_t fre,uint16_t Phase);
    /**
     * @brief 设置PSK调制
     *
     * @param Channel   通道对应值
     * @param Nlevel    调制阶数，对应有2、4、8、16可填。
     * @param data      数据缓存区
     * @param fre       输出频率，范围0~200 000 000 Hz
     *
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setPSK(AD9959_Channel Channel, uint8_t Nlevel, uint16_t *data,uint32_t Freq);
    /**
     * @brief 设置线性扫频的参数
     *
     * 频点与频点间停留时间 dT = Xsrr*8 单位ns，扫描点数=(起始-结束)/步长
							扫频总时间=总扫描频点数*dT
     *
     * @param Channel       通道对应值
     * @param start_Freq    起始频率，范围0~200 000 000 Hz
     * @param end_Freq      结束频率，范围0~200 000 000 Hz
     * @param rise_delta    上升步长频率,0~200 000 000Hz
     * @param fall_delta    下降步长频率,0~200 000 000Hz
     * @param rise_slope    上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
     * @param Ampli         输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
     * @param Phase         输出相位,范围：0~16383(对应角度：0°~360°)
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setFreSweep(AD9959_Channel Channel, uint32_t start_Freq, uint32_t end_Freq, uint32_t rise_delta, uint32_t fall_delta, uint8_t rise_slope, uint8_t fall_slope, uint16_t Ampli, uint16_t Phase);
    /**
     * @brief 设置线性扫幅的参数
     *
     * 频点与频点间停留时间 dT = Xsrr*8 单位ns，扫描点数=(起始-结束)/步长
                            扫频总时间=总扫描频点数*dT
     *
     * @param Channel       通道对应值
     * @param start_Ampli   起始幅度，控制值0~1023对应输出幅度0~500mVpp左右
     * @param end_Ampli     结束幅度
     * @param rise_delta    上升步长频率,0~200 000 000Hz
     * @param fall_delta    下降步长频率,0~200 000 000Hz
     * @param rise_slope    上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
     * @param fre           输出频率，范围0~200 000 000 Hz
     * @param Phase         输出相位,范围：0~16383(对应角度：0°~360°)
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setAmpSweep(AD9959_Channel Channel, uint32_t start_Ampli, uint32_t end_Ampli, uint32_t rise_delta, uint32_t fall_delta, uint8_t rise_slope, uint8_t fall_slope, uint32_t fre,uint16_t Phase);
    /**
     * @brief 设置线性扫相的参数
     *
     * 频点与频点间停留时间 dT = Xsrr*8 单位ns，扫描点数=(起始-结束)/步长
                            扫频总时间=总扫描频点数*dT
     *
     * @param Channel       通道对应值
     * @param start_Phase   起始相位，范围：0~16383(对应角度：0°~360°)
     * @param end_Phase     结束相位
     * @param rise_delta    上升步长频率,0~200 000 000Hz
     * @param fall_delta    下降步长频率,0~200 000 000Hz
     * @param rise_slope    上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
     * @param fre           输出频率，范围0~200 000 000 Hz
     * @param Ampli         输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
     * @return AD9959&  返回AD9959对象的引用
     */
    AD9959& setPhaseSweep(AD9959_Channel Channel, uint16_t start_Phase, uint16_t end_Phase, uint16_t rise_delta, uint16_t fall_delta, uint8_t rise_slope, uint8_t fall_slope, uint32_t fre, uint16_t Ampli);
    /**
     * @brief 调制引脚复位
     *
     */
    inline void reset_SK(void);
    /**
     * @brief 八、十六阶调制引脚更新
     *
     * @param Code 三位或四位二进制数，对应八阶、十六阶的通道Word
     *
     */
    inline void update_SK(uint8_t Code);
    /**
     * @brief 四阶调制引脚更新
     *
     * @param Code0 二位二进制数，对应设定的第一个通道Word
     * @param Code1 二位二进制数，对应设定的第二个通道Word
     *
     */
    inline void update_SK(uint8_t Code0, uint8_t Code1);
    /**
     * @brief 二阶调制引脚更新
     *
     * @param Code0 一位二进制数，对应设定的第一个通道Word
     * @param Code1 一位二进制数，对应设定的第二个通道Word
     * @param Code2 一位二进制数，对应设定的第三个通道Word
     * @param Code3 一位二进制数，对应设定的第四个通道Word
     */
    inline void update_SK(uint8_t Code0, uint8_t Code1, uint8_t Code2, uint8_t Code3);
};

inline void AD9959::reset_SK(void) {
    ps0_gpio = 0;
    ps1_gpio = 0;
    ps2_gpio = 0;
    ps3_gpio = 0;
}

inline void AD9959::update_SK(uint8_t Code) {
    ps3_gpio = Code & 0b0001;
    ps2_gpio = Code & 0b0010;
    ps1_gpio = Code & 0b0100;
    ps0_gpio = Code & 0b1000;
}

inline void AD9959::update_SK(uint8_t Code0, uint8_t Code1)
{
    ps1_gpio = Code0 & 0b01;
    ps0_gpio = Code0 & 0b10;

    ps3_gpio = Code1 & 0b01;
    ps2_gpio = Code1 & 0b10;
}

inline void AD9959::update_SK(uint8_t Code0, uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
    ps0_gpio = Code0;
    ps1_gpio = Code1;
    ps2_gpio = Code2;
    ps3_gpio = Code3;
}

AD9959& AD9959_Config(AD9959& ad9959, AD9959_Channel Channel, uint32_t Freq, uint16_t Ampli, uint16_t Phase);

#endif //__HARDWARE_AD9959_H__
