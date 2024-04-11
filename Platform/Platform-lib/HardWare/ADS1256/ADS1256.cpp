#include "Platform-lib/HardWare/ADS1256/ADS1256.hpp"
#include "Platform-drivers/STM32/Delay/Delay.hpp"
#include "Platform-drivers/STM32/System/Sys.hpp"

using namespace STM32;


/*
	ADS1256基本特性:
	1、模拟部分供电5V;
	2、SPI数字接口电平：3.3V
	3、PGA设置范围： 1、2、4、8、16、32、64、
	4、参考电压2.5V (推荐缺省的，外置的）
	5、输入电压范围：PGA = 1 时, 可输入正负5V
	6. 自动校准 （当设置了PGA,BUF使能、数据采样率时，会启动自校准)
	7. 输入的缓冲器可设置启用和关闭（一般选启用）


	外部晶振频率 = 7.68MHz,
		时钟频率 tCLK = 1/7.68M = 0.13uS
		输出数据周期 tDATA =  1 / 30K = 0.033mS  (按30Ksps计算)

	对SPI的时钟速度要求: (ads1256.pdf page 6)
		最快 4个tCLK = 0.52uS
		最慢 10个tDATA = 0.3mS (按 30Ksps 计算)

		SCL高电平和低电平持续时间最小 200ns

	RREG, WREG, RDATA 命令之后，需要延迟 4 * tCLK = 0.52uS;
	RDATAC, RESET, SYNC 命令之后，需要延迟 24 * tCLK = 3.12uS;

	实际测试，在3.3V上电后, 即使不做任何配置，ADS125的DRDY 口线即开始输出脉冲信号（2.6us高,33.4低，频率30KHz）
*/

/*
	调试记录
	(1) 设置寄存器时，SCK过快导致芯片不能每次都收到数据。原因: 发送的相邻的字节之间需要延迟一小段时间.
	(2) 连续复位CPU时，偶尔出现芯片输出采样率异常。
*/
/* 寄存器定义： Table 23. Register Map --- ADS1256数据手册第30页 */
enum
{
    /* 寄存器地址， 后面是复位后缺省值 */
    REG_STATUS = 0,	// x1H
    REG_MUX    = 1, // 01H
    REG_ADCON  = 2, // 20H
    REG_DRATE  = 3, // F0H
    REG_IO     = 4, // E0H
    REG_OFC0   = 5, // xxH
    REG_OFC1   = 6, // xxH
    REG_OFC2   = 7, // xxH
    REG_FSC0   = 8, // xxH
    REG_FSC1   = 9, // xxH
    REG_FSC2   = 10, // xxH
};

/* 命令定义： TTable 24. Command Definitions --- ADS1256数据手册第34页 */
enum
{
    CMD_WAKEUP  = 0x00,	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
    CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
    CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
    CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
    CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
    CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
    CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
    CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
    CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
    CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
    CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
    CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
    CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
    CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};

static const uint8_t s_tabDataRate[ADS1256_DRATE_MAX] =
{
        0xF0,		/* 复位时缺省值 */
        0xE0,
        0xD0,
        0xC0,
        0xB0,
        0xA1,
        0x92,
        0x82,
        0x72,
        0x63,
        0x53,
        0x43,
        0x33,
        0x20,
        0x13,
        0x03
};

static uint8_t ADS1256_GetPGAGain(ADS1256_GAIN Gain)
{
    uint8_t PGA;
    switch(Gain)
    {
        case ADS1256_GAIN_1 :
            PGA = 1 ;
            break;
        case ADS1256_GAIN_2 :
            PGA = 2 ;
            break;
        case ADS1256_GAIN_4 :
            PGA = 4 ;
            break;
        case ADS1256_GAIN_8 :
            PGA = 8 ;
            break;
        case ADS1256_GAIN_16 :
            PGA = 16 ;
            break;
        case ADS1256_GAIN_32 :
            PGA = 32 ;
            break;
        case ADS1256_GAIN_64 :
            PGA = 64 ;
            break;
    }
    return PGA;
}


ADS1256::ADS1256(GPIO SCK_GPIO, GPIO DIN_GPIO, GPIO CS_GPIO, GPIO DRDY_GPIO,
                 GPIO DOUT_GPIO, GPIO SYNC_GPIO, GPIO RST_GPIO, Platform_GPIOEXTI_CallBack_fun Callback)
                 : sck_gpio(SCK_GPIO), din_gpio(DIN_GPIO), cs_gpio(CS_GPIO),
                   dout_gpio(DOUT_GPIO), sync_gpio(SYNC_GPIO), rst_gpio(RST_GPIO)
{
    sck_gpio.setConfig( pullno, out, pp, very_fast);
    din_gpio.setConfig( pullno, out, pp, very_fast);
    cs_gpio.setConfig( pullno, out, pp, very_fast);
    sync_gpio.setConfig( pullno, out, pp, very_fast);
    rst_gpio.setConfig( pullno, out, pp, very_fast);

    dout_gpio.setConfig( pullno,in, pp, very_fast);
    drdy_gpio = GPIO_EXTI(DRDY_GPIO, falling, Callback);
    // drdy_gpio.disableIT();

    sync_gpio = 1;
    rst_gpio = 1;

    cs_gpio = 1;
    sck_gpio = 0;
    din_gpio = 1;
}

void ADS1256::waitDRDY(void) {
    uint32_t i;

    for (i = 0; i < 40000000; i++)
    {
        if (!drdy_gpio.read())
        {
            break;
        }
    }
    if (i >= 40000000)
    {
		/* 等待超时 */
    }
}

void ADS1256::delaySCLK(void)
{
    __IO uint16_t i;

    /*
        取 5 时，实测高电平200ns, 低电平250ns <-- 不稳定
        取 10 以上，可以正常工作， 低电平400ns 高定400ns <--- 稳定
    */
    for (i = 0; i < 30; i++)
    {
        __NOP();
    }
}

void ADS1256::send8Bit(uint8_t _data) {
    uint8_t i;

    /* 连续发送多个字节时，需要延迟一下 */
    delaySCLK();
    delaySCLK();

    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
    for(i = 0; i < 8; i++)
    {
        if (_data & 0x80)
        {
            din_gpio = 1;
        }
        else
        {
            din_gpio = 0;
        }
        sck_gpio = 1;

        delaySCLK();
        _data <<= 1;
        sck_gpio = 0;			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
        delaySCLK();
    }
}

uint8_t ADS1256::readReg(uint8_t _RegID) {
    uint8_t read;

    cs_gpio = 0;	/* SPI片选 = 0 */
    send8Bit(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
    send8Bit(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */

    delayDATA();	/* 必须延迟才能读取芯片返回数据 */

    read = recive8Bit();	/* 读寄存器值 */
    cs_gpio = 1;	/* SPI片选 = 1 */

    return read;
}

uint8_t ADS1256::recive8Bit(void) {
    uint8_t i;
    uint8_t read = 0;

    delaySCLK();
    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
    for (i = 0; i < 8; i++)
    {
        sck_gpio = 1;
        delaySCLK();
        read = read<<1;
        sck_gpio = 0;
        if (dout_gpio.read())
        {
            read++;
        }
        delaySCLK();
    }
    return read;
}

void ADS1256::resetHard(void)
{
    /* ADS1256数据手册第7页 */
    rst_gpio = 0;			/* 复位 */
    delay_us(5);
    rst_gpio = 1;
    delay_us(5);
}

void ADS1256::delayDATA(void) {
    delay_us(10);
}

void ADS1256::stop(void) {
    drdy_gpio.disableIT();
}

void ADS1256::writeReg(uint8_t _RegID, uint8_t _RegValue) {
    cs_gpio = 0;	/* SPI片选 = 0 */
    send8Bit(CMD_WREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
    send8Bit(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */

    send8Bit(_RegValue);	/* 发送寄存器值 */
    cs_gpio = 1;	/* SPI片选 = 1 */
}

void ADS1256::writeCmd(uint8_t _cmd) {
    cs_gpio = 0;	/* SPI片选 = 0 */
    send8Bit(_cmd);
    cs_gpio = 1;	/* SPI片选 = 1 */
}

int32_t ADS1256::readData(void) {
    uint32_t read = 0;

    cs_gpio = 0;	/* SPI片选 = 0 */

    send8Bit(CMD_RDATA);	/* 读数据的命令 */

    delayDATA();	/* 必须延迟才能读取芯片返回数据 */

    /* 读采样结果，3个字节，高字节在前 */
    read = recive8Bit() << 16;
    read += recive8Bit() << 8;
    read += recive8Bit() << 0;

    delay_us(2);

    cs_gpio = 1;	/* SPI片选 = 1 */

    /* 负数进行扩展。24位有符号数扩展为32位有符号数 */
    if (read & 0x800000)
    {
        read += 0xFF000000;
    }

    return (int32_t)read;
}

void ADS1256::setChannal(uint8_t _ch) {
    /*
    Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
    0000 = AIN0 (default)
    0001 = AIN1
    0010 = AIN2 (ADS1256 only)
    0011 = AIN3 (ADS1256 only)
    0100 = AIN4 (ADS1256 only)
    0101 = AIN5 (ADS1256 only)
    0110 = AIN6 (ADS1256 only)
    0111 = AIN7 (ADS1256 only)
    1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

    NOTE: When using an ADS1255 make sure to only select the available inputs.

    Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
    0000 = AIN0
    0001 = AIN1 (default)
    0010 = AIN2 (ADS1256 only)
    0011 = AIN3 (ADS1256 only)
    0100 = AIN4 (ADS1256 only)
    0101 = AIN5 (ADS1256 only)
    0110 = AIN6 (ADS1256 only)
    0111 = AIN7 (ADS1256 only)
    1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
*/
    if (_ch > 7)
    {
        return;
    }
    writeReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN 固定接 AINCOM */
}

void ADS1256::setDiffChannal(uint8_t _ch)
{
    /*
    Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
        0000 = AIN0 (default)
        0001 = AIN1
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

        NOTE: When using an ADS1255 make sure to only select the available inputs.

    Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
        0000 = AIN0
        0001 = AIN1 (default)
        0010 = AIN2 (ADS1256 only)
        0011 = AIN3 (ADS1256 only)
        0100 = AIN4 (ADS1256 only)
        0101 = AIN5 (ADS1256 only)
        0110 = AIN6 (ADS1256 only)
        0111 = AIN7 (ADS1256 only)
        1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
    */
    if (_ch == 0)
    {
        writeReg(REG_MUX, (0 << 4) | 1);	/* 差分输入 AIN0， AIN1 */
    }
    else if (_ch == 1)
    {
        writeReg(REG_MUX, (2 << 4) | 3);	/* 差分输入 AIN2， AIN3 */
    }
    else if (_ch == 2)
    {
        writeReg(REG_MUX, (4 << 4) | 5);	/* 差分输入 AIN4， AIN5 */
    }
    else if (_ch == 3)
    {
        writeReg(REG_MUX, (6 << 4) | 7);	/* 差分输入 AIN6， AIN7 */
    }
}

uint8_t ADS1256::readChipID(void) {
    uint8_t id;

    waitDRDY();
    id = readReg(REG_STATUS);
    return (id >> 4);
}

ADS1256 &ADS1256::config(ADS1256_GAIN _gain, ADS1256_DRATE _drate) {

    Gain = ADS1256_GetPGAGain(_gain);

    stop();

    resetHard();		/* 硬件复位 */

    waitDRDY();

    {
        uint8_t buf[4];		/* 暂存ADS1256 寄存器配置参数，之后连续写4个寄存器 */

        /* 状态寄存器定义
            Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

            Bit 3 ORDER: Data Output Bit Order
                0 = Most Significant Bit First (default)
                1 = Least Significant Bit First
            Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
            byte first. The ORDER bit only controls the bit order of the output data within the byte.

            Bit 2 ACAL : Auto-Calibration
                0 = Auto-Calibration Disabled (default)
                1 = Auto-Calibration Enabled
            When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
            the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
            values.

            Bit 1 BUFEN: Analog Input Buffer Enable
                0 = Buffer Disabled (default)
                1 = Buffer Enabled

            Bit 0 DRDY :  Data Ready (Read Only)
                This bit duplicates the state of the DRDY pin.

            ACAL=1使能自校准功能。当 PGA，BUFEEN, DRATE改变时会启动自校准
        */
        buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
        //writeReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

        buf[1] = 0x08;	/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */

        /*	ADCON: A/D Control Register (Address 02h)
            Bit 7 Reserved, always 0 (Read Only)
            Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
                00 = Clock Out OFF
                01 = Clock Out Frequency = fCLKIN (default)
                10 = Clock Out Frequency = fCLKIN/2
                11 = Clock Out Frequency = fCLKIN/4
                When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

            Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
                00 = Sensor Detect OFF (default)
                01 = Sensor Detect Current = 0.5 μ A
                10 = Sensor Detect Current = 2 μ A
                11 = Sensor Detect Current = 10μ A
                The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
                ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

            Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
                000 = 1 (default)
                001 = 2
                010 = 4
                011 = 8
                100 = 16
                101 = 32
                110 = 64
                111 = 64
        */
        buf[2] = (0 << 5) | (0 << 2) | (_gain << 1);
        //writeReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 1));	/* 选择1;1增益, 输入正负5V */

        /* 因为切换通道和读数据耗时 123uS, 因此扫描中断模式工作时，最大速率 = DRATE_1000SPS */
        buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	/* 选择数据输出速率 */

        cs_gpio = 0;	/* SPI片选 = 0 */
        send8Bit(CMD_WREG | 0);	/* 写寄存器的命令, 并发送寄存器地址 */
        send8Bit(0x03);			/* 寄存器个数 - 1, 此处3表示写4个寄存器 */

        send8Bit(buf[0]);	/* 设置状态寄存器 */
        send8Bit(buf[1]);	/* 设置输入通道参数 */
        send8Bit(buf[2]);	/* 设置ADCON控制寄存器，增益 */
        send8Bit(buf[3]);	/* 设置ADC采样率 */

        cs_gpio = 1;	/* SPI片选 = 1 */
    }

    delay_us(50);
    return *this;
}

void ADS1256::start(ADS1256_MODE mode) {
    Mode = mode;

    drdy_gpio.enableIT();

    /* 开始扫描前, 清零结果缓冲区 */
    uint8_t i;

    Channel = 0;

    for (i = 0; i < 8; i++)
    {
        AdcNow[i] = 0;
    }

}

void ADS1256::irq(void) {
    if (LL_EXTI_IsActiveFlag_0_31(drdy_gpio.getExtiLine()) != RESET)
    {
        LL_EXTI_ClearFlag_0_31(drdy_gpio.getExtiLine());
    }

    if (Mode == ADS1256_MODE_Single) /* 单端8路扫描 */
    {
        /* 读取采集结构，保存在全局变量 */
        setChannal(Channel);	/* 切换模拟通道 */
        delay_us(5);

        writeCmd(CMD_SYNC);
        delay_us(5);

        writeCmd(CMD_WAKEUP);
        delay_us(25);

        if (Channel == 0)
        {
            AdcNow[7] = readData();	/* 注意保存的是上一个通道的数据 */
        }
        else
        {
            AdcNow[Channel-1] = readData();	/* 注意保存的是上一个通道的数据 */
        }

        if (++Channel >= 8)
        {
            Channel = 0;
            flag_finished = true;
        }

    }

    else    /* 差分4路扫描 */
    {
        /* 读取采集结构，保存在全局变量 */
        setDiffChannal(Channel);	/* 切换模拟通道 */
        delay_us(5);

        writeCmd(CMD_SYNC);
        delay_us(5);

        writeCmd(CMD_WAKEUP);
        delay_us(25);

        if (Channel == 0)
        {
            AdcNow[3] = readData();	/* 注意保存的是上一个通道的数据 */
        }
        else
        {
            AdcNow[Channel-1] = readData();	/* 注意保存的是上一个通道的数据 */
        }

        if (++Channel >= 4)
        {
            Channel = 0;
            flag_finished = true;
        }
    }


}

int32_t ADS1256::getData(uint8_t _ch) {
    int32_t iTemp;

    if (_ch > 7)
    {
        return 0;
    }

    DISABLE_INT();  			/* 关中断 */

    iTemp = AdcNow[_ch];

    ENABLE_INT();  				/* 开中断 */

    return iTemp;
}

double ADS1256::getVolt(uint8_t _ch) {
    return (float)(((int64_t)getData(_ch) * ADS1256_VREF) / ADS1256_VREF_Encode) / 1000000.0;
}

bool ADS1256::isFinished(void) {

    if (flag_finished) {
        flag_finished = false;
        return true;
    }
    return false;
}

int32_t ADS1256::readADC(uint8_t _ch) {
        /* ADS1256 数据手册第21页 */

#if 1	/* 对于30Ksps 采样速率 */
    int32_t read;

	while (!drdy_gpio.read());	/* 等待 DRDY 高 */
	while (drdy_gpio.read());	/* 等待 DRDY 低 */

	setChannal(_ch);	/* 切换模拟通道 */
	delay_us(5);

	writeCmd(CMD_SYNC);
    delay_us(5);

	writeCmd(CMD_WAKEUP);  /* 正常情况下，这个时候 DRDY 已经为高 */
    delay_us(25);

	read =  (int32_t)readData();

    while (!drdy_gpio.read());	/* 等待 DRDY 高 */
    while (drdy_gpio.read());	/* 等待 DRDY 低 */

	read =  (int32_t)readData();

	return read;
#else
        //while (DRDY_IS_LOW());

        /* ADS1256 数据手册第21页 */
        waitDRDY();		/* 等待 DRDY = 0 */

        setChannal(_ch);	/* 切换模拟通道 */
        delay_us(5);

        writeCmd(CMD_SYNC);
        delay_us(5);

        writeCmd(CMD_WAKEUP);
        delay_us(25);

        //waitDRDY();		/* 等待 DRDY = 0 */

        return (int32_t)readData();
#endif
}

uint8_t ADS1256::getGain(void) {
    return Gain;
}


