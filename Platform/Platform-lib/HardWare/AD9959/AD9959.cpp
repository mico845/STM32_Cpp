#include "Platform-lib/HardWare/AD9959/AD9959.hpp"
#include "Platform-drivers/STM32/Delay/Delay.hpp"

using namespace STM32;

uint8_t FR1_DATA[3] = {0xD0,0x00,0x00};//VCO gain control[23]=1系统时钟高于255Mhz; PLL[22:18]=10100,20倍频,20*25M=500MHZ; Charge pump control = 75uA

uint8_t FR2_DATA[2] = {0x00,0x00};	// 双方向扫描，即从起始值扫到结束值后，又从结束值扫到起始值
//uint8_t FR2_DATA[2] = {0x80,0x00};// 单方向扫描，即从起始值扫到结束值后，又从起始值扫到结束值，以此类推

double ACC_FRE_FACTOR = 8.589934592;	//频率因子8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)

uint8_t CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302	   //通道功能寄存器

//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器

#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器

#define LSRR_ADD  0x07   //LSR 线性扫描斜率寄存器
#define RDW_ADD   0x08   //RDW 上升扫描增量寄存器
#define FDW_ADD   0x09   //FDW 下降扫描增量寄存器

#define PROFILE_ADDR_BASE   0x0A   //Profile寄存器,配置文件寄存器起始地址

//FR1[9:8] 调制电平选择位
#define LEVEL_MOD_2  	0x00//2电平调制 2阶调制
#define LEVEL_MOD_4		0x01//4电平调制	4阶调制
#define LEVEL_MOD_8		0x02//8电平调制	8阶调制
#define LEVEL_MOD_16	0x03//16电平调制	16阶调制

//CFR[23:22]  幅频相位（AFP）选择位
#define	DISABLE_Mod		0x00	//00	调制已禁用
#define	ASK 					0x40	//01	振幅调制，幅移键控
#define	FSK 					0x80	//10	频率调制，频移键控
#define	PSK 					0xc0	//11	相位调制，相移键控

//（CFR[14]）线性扫描启用 sweep enable
#define	SWEEP_ENABLE	0x40	//1	启用
#define	SWEEP_DISABLE	0x00	//0	不启用


AD9959::AD9959(GPIO CS_GPIO, GPIO SCLK_GPIO, GPIO UPDATE_GPIO, GPIO PS0_GPIO,
               GPIO PS1_GPIO, GPIO PS2_GPIO, GPIO PS3_GPIO, GPIO SDIO0_GPIO,
               GPIO SDIO1_GPIO, GPIO SDIO2_GPIO, GPIO SDIO3_GPIO, GPIO PWR_GPIO,
               GPIO RESET_GPIO) :
               cs_gpio(CS_GPIO), sclk_gpio(SCLK_GPIO), update_gpio(UPDATE_GPIO), ps0_gpio(PS0_GPIO),
               ps1_gpio(PS1_GPIO), ps2_gpio(PS2_GPIO), ps3_gpio(PS3_GPIO), sdio0_gpio(SDIO0_GPIO),
               sdio1_gpio(SDIO1_GPIO), sdio2_gpio(SDIO2_GPIO), sdio3_gpio(SDIO3_GPIO), pwr_gpio(PWR_GPIO),
               reset_gpio(RESET_GPIO)
{
    intserve();  //IO口电平状态初始化
    intReset();  //AD9959复位
    //初始化功能寄存器
    writeData(FR1_ADD, 3, FR1_DATA);//写功能寄存器1
    writeData(FR2_ADD, 2, FR2_DATA);
}

void AD9959::intserve(void) {
    pwr_gpio = 0;
    cs_gpio = 1;
    sclk_gpio = 0;
    update_gpio = 0;
    ps0_gpio = 0;
    ps1_gpio = 0;
    ps2_gpio = 0;
    ps3_gpio = 0;
    sdio0_gpio  = 0;
    sdio1_gpio = 0;
    sdio2_gpio = 0;
    sdio3_gpio = 0;
}

void AD9959::intReset(void) {
    reset_gpio = 0;
    delay_us(1);
    reset_gpio = 1;
    delay_us(30);
    reset_gpio = 0;
}

void AD9959::update(void) {
    update_gpio = 0;
    delay_us(2);
//	delay1(2);
    update_gpio = 1;
    delay_us(4);
    //delay1(4);
    update_gpio = 0;
}

void AD9959::writeData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData) {
    uint8_t	ControlValue = 0;
    uint8_t	ValueToWrite = 0;
    uint8_t	RegisterIndex = 0;
    uint8_t	i = 0;

    ControlValue = RegisterAddress;
//写入地址
    sclk_gpio = 0;
    cs_gpio = 0;
    for(i=0; i<8; i++)
    {
        sclk_gpio = 0;
        if(0x80 == (ControlValue & 0x80))
            sdio0_gpio = 1;
        else
            sdio0_gpio = 0;
        sclk_gpio = 1;
        ControlValue <<= 1;
    }
    sclk_gpio = 0;
//写入数据
    for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
    {
        ValueToWrite = RegisterData[RegisterIndex];
        for (i=0; i<8; i++)
        {
            sclk_gpio = 0;
            if(0x80 == (ValueToWrite & 0x80))
                sdio0_gpio = 1;
            else
                sdio0_gpio= 0;
            sclk_gpio = 1;
            ValueToWrite <<= 1;
        }
        sclk_gpio = 0;
    }
    cs_gpio = 1;
}

void AD9959::write_CFTW0(uint32_t fre) {
    uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
    uint32_t Temp;
    Temp=(uint32_t)fre*ACC_FRE_FACTOR;
    CFTW0_DATA[3]=(uint8_t)Temp;
    CFTW0_DATA[2]=(uint8_t)(Temp>>8);
    CFTW0_DATA[1]=(uint8_t)(Temp>>16);
    CFTW0_DATA[0]=(uint8_t)(Temp>>24);
    writeData(CFTW0_ADD, 4, CFTW0_DATA);//CTW0 address 0x04
}

void AD9959::write_ACR(uint16_t Ampli) {
    uint32_t A_temp=0;
    uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout
    A_temp=Ampli|0x1000;

    ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据
    ACR_DATA[2] = (uint8_t)A_temp;  //低位数据
    writeData(ACR_ADD, 3, ACR_DATA); //ACR address 0x06.CHn设定幅度
}

void AD9959::write_CPOW0(uint16_t Phase) {
    uint8_t CPOW0_data[2] = {0x00,0x00};
    CPOW0_data[1]=(uint8_t)Phase;
    CPOW0_data[0]=(uint8_t)(Phase>>8);

    writeData(CPOW0_ADD, 2, CPOW0_data);//CPOW0 address 0x05.CHn设定相位
}

void AD9959::write_LSRR(uint8_t rsrr, uint8_t fsrr) {
    uint8_t LSRR_data[2]={0x00,0x00};

    LSRR_data[1]=rsrr;
    LSRR_data[0]=fsrr;//高8位下降斜率

    writeData(LSRR_ADD, 2, LSRR_data);
}

void AD9959::write_RDW(uint32_t r_delta) {
    uint8_t RDW_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

    RDW_data[3]=(uint8_t)r_delta;
    RDW_data[2]=(uint8_t)(r_delta>>8);
    RDW_data[1]=(uint8_t)(r_delta>>16);
    RDW_data[0]=(uint8_t)(r_delta>>24);
    writeData(RDW_ADD, 4, RDW_data);
}

void AD9959::write_FDW(uint32_t f_delta) {
    uint8_t FDW_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

    FDW_data[3]=(uint8_t)f_delta;
    FDW_data[2]=(uint8_t)(f_delta>>8);
    FDW_data[1]=(uint8_t)(f_delta>>16);
    FDW_data[0]=(uint8_t)(f_delta>>24);
    writeData(FDW_ADD, 4, FDW_data);
}

void AD9959::write_Profile_Fre(uint8_t profile, uint32_t data) {
    uint8_t profileAddr;
    uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量
    uint32_t Temp;
    Temp=(uint32_t)data*ACC_FRE_FACTOR;	   //将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
    Profile_data[3]=(uint8_t)Temp;
    Profile_data[2]=(uint8_t)(Temp>>8);
    Profile_data[1]=(uint8_t)(Temp>>16);
    Profile_data[0]=(uint8_t)(Temp>>24);
    profileAddr = PROFILE_ADDR_BASE + profile;

    writeData(profileAddr, 4, Profile_data);
}

void AD9959::write_Profile_Ampli(uint8_t profile, uint16_t data) {
    uint8_t profileAddr;
    uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

    //为幅度调制时幅度数据为[31:22]位
    Profile_data[1]=(uint8_t)(data<<6);//[23:22]
    Profile_data[0]=(uint8_t)(data>>2);//[31:24]

    profileAddr = PROFILE_ADDR_BASE + profile;

    writeData(profileAddr, 4, Profile_data);//写入32位数据
}

void AD9959::write_Profile_Phase(uint8_t profile, uint16_t data) {
    uint8_t profileAddr;
    uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

    //为相位调制时相位数据为[31:18]位
    Profile_data[1]=(uint8_t)(data<<2);//[23:18]
    Profile_data[0]=(uint8_t)(data>>6);//[31:24]

    profileAddr = PROFILE_ADDR_BASE + profile;

    writeData(profileAddr, 4, Profile_data);//写入32位数据
}

AD9959& AD9959::setFre(AD9959_Channel Channel, uint32_t Freq) {
    uint8_t CHANNEL[1] = {0x00};

    CHANNEL[0]=Channel;
    writeData(CSR_ADD, 1, CHANNEL);//控制寄存器写入CHn通道，选择CHn
    write_CFTW0(Freq);//输出CHn设定频率

    return *this;
}

AD9959& AD9959::setAmp(AD9959_Channel Channel, uint16_t Ampli) {
    uint8_t CHANNEL[1] = {0x00};

    CHANNEL[0]=Channel;
    writeData(CSR_ADD, 1, CHANNEL); //控制寄存器写入CHn通道，选择CHn
    write_ACR(Ampli);

    return *this;
}

AD9959& AD9959::setPhase(AD9959_Channel Channel, uint16_t Phase) {
    uint8_t CHANNEL[1] = {0x00};
    CHANNEL[0]=Channel;

    writeData(CSR_ADD, 1, CHANNEL); //控制寄存器写入CHn通道，选择CHn
    write_CPOW0(Phase);//CHn设定相位

    return *this;
}

void AD9959::SetModulation(AD9959_Channel Channel, uint8_t Modulation, uint8_t Sweep_en, uint8_t Nlevel) {
    uint8_t i=0;
    uint8_t CHANNEL[1]={0x00};
    uint8_t FR1_data[3];
    uint8_t FR2_data[2];
    uint8_t CFR_data[3];
    for(i=0;i<3;i++)//设置默认值
    {
        FR1_data[i]=FR1_DATA[i];
        CFR_data[i]=CFR_DATA[i];
    }
    FR2_data[0]=FR2_DATA[0];
    FR2_data[1]=FR2_DATA[1];

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

    FR1_data[1]=Nlevel;
    CFR_data[0]=Modulation;
    CFR_data[1]|=Sweep_en;
    CFR_data[2]=0x00;

    if(Channel!=0)//有通道被使能
    {
        writeData(FR1_ADD,3,FR1_data);//写功能寄存器1
        writeData(FR2_ADD,2,FR2_data);//写功能寄存器1
        writeData(CFR_ADD,3,CFR_data);//写通道功能寄存器
    }
}

AD9959& AD9959::setFSK(AD9959_Channel Channel, uint8_t Nlevel, uint32_t *data, uint16_t Phase) {
    uint8_t nLevel = 0x00;
    uint8_t i=0;
    uint8_t CHANNEL[1]={0x00};
    if (Nlevel == 2)
        nLevel = LEVEL_MOD_2;
    else if(Nlevel == 4)
        nLevel = LEVEL_MOD_4;
    else if (Nlevel == 8)
        nLevel = LEVEL_MOD_8;
    else if (Nlevel == 16)
        nLevel = LEVEL_MOD_16;
    SetModulation(Channel, FSK, SWEEP_DISABLE, nLevel);

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
    write_CPOW0(Phase);//设置相位

    write_CFTW0(data[0]);

    for(i=0;i<15;i++)
    {
        if(i < Nlevel-1)
            write_Profile_Fre(i, data[i+1]);
        else
            write_Profile_Fre(i, 0);
    }

    return *this;

}

AD9959& AD9959::setASK(AD9959_Channel Channel, uint8_t Nlevel, uint16_t *data, uint32_t fre, uint16_t Phase) {
    uint8_t nLevel = 0x00;
    uint8_t i;
    uint8_t CHANNEL[1]={0x00};
    if (Nlevel == 2)
        nLevel = LEVEL_MOD_2;
    else if(Nlevel == 4)
        nLevel = LEVEL_MOD_4;
    else if (Nlevel == 8)
        nLevel = LEVEL_MOD_8;
    else if (Nlevel == 16)
        nLevel = LEVEL_MOD_16;
    SetModulation(Channel, ASK, SWEEP_DISABLE, nLevel);

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
    write_CFTW0(fre);//设置频率
    write_CPOW0(Phase);//设置相位

    write_ACR(data[0]);
    for(i=0;i<15;i++)
    {
        if(i < Nlevel-1)
            write_Profile_Ampli(i, data[i+1]);
        else
            write_Profile_Ampli(i, 0);
    }

    return *this;
}

AD9959 &AD9959::setPSK(AD9959_Channel Channel, uint8_t Nlevel, uint16_t *data, uint32_t Freq) {
    uint8_t nLevel = 0x00;
    uint8_t i=0;
    uint8_t CHANNEL[1]={0x00};
    if (Nlevel == 2)
        nLevel = LEVEL_MOD_2;
    else if(Nlevel == 4)
        nLevel = LEVEL_MOD_4;
    else if (Nlevel == 8)
        nLevel = LEVEL_MOD_8;
    else if (Nlevel == 16)
        nLevel = LEVEL_MOD_16;
    SetModulation(Channel, PSK, SWEEP_DISABLE, nLevel);

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
    write_CFTW0(Freq);

    write_CPOW0(data[0]);
    for(i=0;i<15;i++)
    {
        if(i < Nlevel-1)
            write_Profile_Phase(i, data[i+1]);
        else
            write_Profile_Phase(i, 0);
    }

    return *this;
}

AD9959 &
AD9959::setFreSweep(AD9959_Channel Channel, uint32_t start_Freq, uint32_t end_Freq, uint32_t rise_delta, uint32_t fall_delta,
                    uint8_t rise_slope, uint8_t fall_slope, uint16_t Ampli, uint16_t Phase) {
    uint8_t CHANNEL[1]={0x00};
    uint32_t Fer_data=0;

    SetModulation(Channel, FSK, SWEEP_ENABLE, LEVEL_MOD_2);

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

    write_CPOW0(Phase);//设置相位
    write_ACR(Ampli); //幅度设置

    write_LSRR(rise_slope, fall_slope);//斜率

    Fer_data= (uint32_t)rise_delta * ACC_FRE_FACTOR;	 //频率转换成控制字
    write_RDW(Fer_data);//上升步长

    Fer_data= (uint32_t)fall_delta * ACC_FRE_FACTOR;
    write_FDW(Fer_data);//下降步长

    write_CFTW0(start_Freq);//起始频率
    write_Profile_Fre(0, end_Freq);//结束频率
    return *this;
}

AD9959 &AD9959::setAmpSweep(AD9959_Channel Channel, uint32_t start_Ampli, uint32_t end_Ampli, uint32_t rise_delta,
                            uint32_t fall_delta, uint8_t rise_slope, uint8_t fall_slope, uint32_t fre, uint16_t Phase) {

    uint8_t CHANNEL[1]={0x00};
    uint8_t ACR_data[3] = {0x00,0x00,0x00};
    SetModulation(Channel, ASK, SWEEP_ENABLE, LEVEL_MOD_2);
    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

    write_CFTW0(fre); //幅度频率
    write_CPOW0(Phase);//设置相位

    write_LSRR(rise_slope, fall_slope);//斜率

    write_RDW(rise_delta<<22);//上升步长

    write_FDW(fall_delta<<22);//下降步长

    ACR_data[1] = (uint8_t)(start_Ampli>>8); //高位数据
    ACR_data[2] = (uint8_t)start_Ampli;  //低位数据
    writeData(ACR_ADD,3,ACR_data); //ACR 设定起始幅度

    write_Profile_Ampli(0, end_Ampli);//结束幅度

    return *this;
}

AD9959& AD9959::setPhaseSweep(AD9959_Channel Channel, uint16_t start_Phase, uint16_t end_Phase, uint16_t rise_delta, uint16_t fall_delta,
                              uint8_t rise_slope, uint8_t fall_slope, uint32_t fre, uint16_t Ampli) {

    uint8_t CHANNEL[1]={0x00};
    SetModulation(Channel, PSK, SWEEP_ENABLE, LEVEL_MOD_2);

    CHANNEL[0]=Channel;
    writeData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

    write_CFTW0(fre); //幅度频率
    write_ACR(Ampli); //幅度设置

    write_LSRR(rise_slope, fall_slope);//斜率

    write_RDW(rise_delta << 18);//上升步长

    write_FDW(fall_delta << 18);//下降步长

    write_CPOW0(start_Phase);//起始相位
    write_Profile_Phase(0, end_Phase);//结束相位

    return *this;
}

AD9959& AD9959_Config(AD9959& ad9959, AD9959_Channel Channel, uint32_t Freq, uint16_t Ampli, uint16_t Phase)
{
    ad9959.setFre(Channel, Freq).setAmp(Channel, Ampli).setPhase(Channel, Phase).update();
    return ad9959;
}