## 一、基本流程
需要添加的头文件路径以及源文件路径:
头文件路径
```
UserApp
Platform/Platform-lib
Platform/Platform-drivers/STM32
```
源文件路径
```
"UserApp/*.*"
"Platform/Platform-drivers/STM32/Platform-Operation/*.*"
"Platform/Platform-lib/*.*"
```
源文件需过滤文件：
```
Core/Src/stm32h7xx_it.c
```



用户需要操作的文件夹为UserApp文件夹
**UserApp**
- [ `common_inc.h` ]    C和C++混编头文件
- [ `Initalize.cpp` ]   初始化
- [ `Irq.cpp` ]         设置中断
- [ `Machine.hpp` ]     声明全局对象
- [ `Main.cpp` ]        主函数

其中着重需要操作的是`Initalize.cpp`、`Machine.hpp`、`Irq.cpp`、`Main.cpp`。

### 一般流程
1. **提前根据需求在STM32CubeMX中进行硬件配置，并生成初始化代码（选择LL库）**
2. 在`Main.cpp`中声明定义，包括对对象的赋值。（一定要在函数中赋值的形式构造对象，不然容易造成操作执行顺序和硬件初始化先后混乱的现象）
```cpp
Timer timer;
void Init(void)
{
    timer = Timer(7, true);
    timer.RegisterCallback(KeyScan10ms).Start(100);
}
void Loop(void)
{

}
```
3. 为其在`Irq.cpp`中添加中断服务函数。
```cpp
TIM_IRQ(7, timer.Irq())
```

### 一般流程
若需要头文件中`extern` 声明全局对象，可以例照以下方式：
1. **提前根据需求在STM32CubeMX中进行硬件配置，并生成初始化代码（选择LL库）**
2. 在`Machine.hpp`声明全局对象
```cpp
extern      Timer        timer;
```
3. 在`Initalize.cpp`中定义，并在`ClassInit`中赋值。（也可以在`Main.cpp`中的`Init`中进行，）
```cpp
Timer           timer;
void ClassInit(void)
{
    timer = Timer(7, true);
}
```
4. 如果有中断需求，在`Irq.cpp`中添加中断服务函数。
```cpp
TIM_IRQ(7, timer.Irq())
```
5. 在`Main.cpp`中使用。
```cpp
void Init(void)
{
    timer.RegisterCallback(KeyScan10ms).Start(100);
}
void Loop(void)
{

}
```
   

## 二、接口操作

...正在制作中

## 三、使用案例
### （一）闪光灯
`Machine.hpp`
```cpp
extern      LED_RGB      led;
extern      Timer        timer;
```
`Irq.cpp`
```cpp
/* TIM */
TIM_IRQ(7, timer.Irq())
```
`Initalize.cpp`
```cpp
LED_RGB         led  ;
Timer           timer;
void ClassInit(void){
    led     =   LED_RGB(GPIO(C, 0, out),
                        GPIO(C, 1, out),
                        GPIO(C, 2, out)
                        );
    timer = Timer(7, true);
}
```

`Main.cpp`
```cpp
// 循环中需要执行的任务
static void Task_Led_Flash(void);

void Init(void)
{
    led.PinkishRed();//初始粉色
    timer.Start(100);
}

void Loop(void)
{
    Task_Led_Flash();
}

static void Task_Led_Flash(void)
{
    static uint8_t count = 0;
    if (timer.Is_Timeout())
        if (count++ == 50) // 10ms * N
        {
            led.Toggle();
            count = 0;
        }
}

```



### （二）按键检测

`Machine.hpp`
```cpp
extern      LED_RGB      led;
extern      Timer        timer;
```
`Irq.cpp`
```cpp
/* TIM */
TIM_IRQ(7, timer.Irq())
```
`Initalize.cpp`
```cpp
LED_RGB         led  ;
Timer           timer;
void ClassInit(void){
    led     =   LED_RGB(GPIO(C, 0, out),
                        GPIO(C, 1, out),
                        GPIO(C, 2, out)
                        );
    timer = Timer(7, true);
}
void HardInit(void)
{
    Key_Init();
}
```
进入`Platform/Platform-lib/OffChip/Key/Key.hpp`
可以更改相关宏定义：
```cpp
//按键数量
#define KEY_HARD_NUM                (1)
//定义KEY1_GPIO
#ifndef KEY1_GPIO_GPIO
    #define KEY1_GPIO_GPIO     (Platform::HardWare::Key::KEY1)  //ID
    #define KEY1_GPIO_PORT     (STM32::STM32_GPIO::C)
    #define KEY1_GPIO_PIN      (13)
    #define KEY1_GPIO_PULL     (STM32::STM32_GPIO::pullup)
#endif
```
并在`Platform/Platform-lib/OffChip/Key/Key.cpp`中的`Key_Init`添加更多的按钮
```cpp
void Key_Init(void)
{
    _key[0] = Key(KEY1_GPIO_GPIO, KEY1_GPIO_PORT, KEY1_GPIO_PIN, KEY1_GPIO_PULL);
    /* 自行添加 */
}
```


`Main.cpp`
```cpp
// 循环中需要执行的任务
static void Task_Key(void);

void Init(void)
{
    led.PinkishRed();//初始粉色
    timer.RegisterCallback(KeyScan10ms).Start(100); //开启一个定时10ms溢满的定时器
}

void Loop(void)
{
    Task_Key();
}

static void Task_Key(void)
{
    static Key_TypeDef key_value;
    key_value = Key_GetValue();
    switch (key_value) {
        case Key1_Down:{
            led.Yellow();
        }break;
        case Key1_Long:{
            led.Cyan();
        }break;
        default:
            break;
    }
}
```

### （三）串口收发
`Machine.hpp`
```cpp
extern      USART        cout;
```
`Irq.cpp`
```cpp
/* USART */
USART_IRQ(1, cout.Irq())
DMA_IRQ(1, 0, cout.Irq_RX_DMA())
DMA_IRQ(1, 1, cout.Irq_TX_DMA())
```
`Initalize.cpp`
```cpp
ALIGN_32B(USART           cout  __AT_SRAM1_);
void ClassInit(void){
    cout  = USART(1,
                  1,1,
                  1,0);
}
```

`Main.cpp`
```cpp
// 循环中需要执行的任务
static void Task_UART_Finished(void);

void Init(void)
{
    cout << "Hello World" << '\n';
    cout.Receive_Start_DMA();
}

void Loop(void)
{
    Task_UART_Finished();
}

static void Task_UART_Finished(void)
{
    static uint8_t buffer[256] = {0};
    if (cout.Is_ReceivedFinished())
    {
        cout.GetReceivedData((uint8_t *)buffer, cout.Get_ReceivedSize());
        cout << (char *)buffer << '\n';
    }
}
```
### （四）ADC采集
`Machine.hpp`
```cpp
extern      ADC          adc;
```
`Irq.cpp`
```cpp
/* ADC */
DMA_IRQ(2, 0, adc.Irq_DMA())
```
`Initalize.cpp`
```cpp
using namespace STM32::STM32_TIM;

ALIGN_32B(ADC             adc   __AT_SRAM4_);
void ClassInit(void){
    adc   = ADC(3, 2, 0, 1, channel1);
}
```

`Main.cpp`
```cpp
// 循环中需要执行的任务
static void Task_ADC_Finished(void);

void Init(void)
{
    adc.Start(1000);
}

void Loop(void)
{
    Task_ADC_Finished();
}

static void Task_ADC_Finished(void)
{
    static uint16_t buffer[1024];
    if (adc.Scan_Data())
    {
        adc.Stop();
        cout << "adc is ok\n";
        if (adc.Get_Data(buffer, adc.Get_DataSize()))
        {
            for (int i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
                cout << i << "  " << buffer[i] << '\n';
            }
        }
        //adc.Start(1000);
    }
}
```

### （五）DAC输出波形
`Machine.hpp`
```cpp
extern      DAC          dac;
```
`Irq.cpp`
```cpp
/* DAC */
DMA_IRQ(2, 1, dac.Irq_DMA())
```
`Initalize.cpp`
```cpp
ALIGN_32B(DAC             dac   __AT_SRAM4_ );
void ClassInit(void){
    dac   = DAC(1, 1, 2, 1, 6);
}
```

`Main.cpp`
```cpp
//需要执行的任务
static void DAC_WaveStart(void);

void Init(void)
{
    DAC_WaveStart();
}

void Loop(void)
{
    
}

static void DAC_WaveStart(void)
{
    #define DAC_WAVE_SIZE (256)
    static uint16_t SinWave_ROM[DAC_WAVE_SIZE];
    generateSineWaveTable(SinWave_ROM, DAC_WAVE_SIZE, 3);
    if (dac.Set_DMABuffer(SinWave_ROM, DAC_WAVE_SIZE))
    dac.Start(1000 * DAC_WAVE_SIZE);
}
```

### （六）DAC模拟DDS
`Machine.hpp`
```cpp
extern      DAC          dac;
extern      DDS          dds;
```
`Irq.cpp`
```cpp
/* DDS */
DMA_IRQ(2, 1, dds.Irq_DMA())
```
`Initalize.cpp`
```cpp
ALIGN_32B(DAC             dac   __AT_SRAM4_ );
DDS             dds;
void ClassInit(void){
    dac   = DAC(1, 1, 2, 1, 6);
    dds   = DDS(&dac);
}
```

`Main.cpp`
```cpp
//需要执行的任务
static void DDS_WaveStart(void);

void Init(void)
{
    DDS_WaveStart();
}

void Loop(void)
{
    
}

static void DDS_WaveStart(void)
{
    #define DDS_ROM_WAVE_SIZE (4096 * 16)
    static uint16_t SinWave_ROM[DDS_ROM_WAVE_SIZE];
    generateSineWaveTable(SinWave_ROM, DDS_ROM_WAVE_SIZE, 3);
    DDS_Config(dds, 4096000,SinWave_ROM, DDS_ROM_WAVE_SIZE, 40000, 0).Start();
}
```

### （七）SPI驱动LCD

`Machine.hpp`
```cpp
extern      LCD_SPI      lcd;
```
`Irq.cpp`
```cpp
/* TIM */
TIM_IRQ(7, timer.Irq())
```
`Initalize.cpp`
```cpp
LED_RGB         led  ;
Timer           timer;
void ClassInit(void){
    lcd   = LCD_SPI(true);
}
void HardInit(void)
{
    lcd.Clear();
}
```
进入`Platform/Platform-lib/OffChip/LCD_SPI/LCD_SPI.hpp`
可以更改相关宏定义：
```cpp
// 定义LCD所使用的SPI
#ifndef LCD_SPI_HANDEL
    #define LCD_SPI_HANDEL  (SPI6)
#endif
//定义LCD_DC
#ifndef LCD_DC_GPIO
    #define LCD_DC_GPIO
    #define LCD_DC_PORT     (STM32::STM32_GPIO::B)
    #define LCD_DC_PIN      (4)
#endif
//定义LCD_RST
#ifndef LCD_RST_GPIO
    #define LCD_RST_GPIO
    #define LCD_RST_PORT    (STM32::STM32_GPIO::D)
    #define LCD_RST_PIN     (4)
#endif
//定义LCD_PWR
#ifndef LCD_PWR_GPIO
    #define LCD_PWR_GPIO
    #define LCD_PWR_PORT    (STM32::STM32_GPIO::D)
    #define LCD_PWR_PIN     (3)
#endif
```

`Main.cpp`
```cpp
using namespace Platform::HardWare;
void Init(void)
{
    lcd << "hello" << TFT_LCD::endl;
    lcd << "world" << TFT_LCD::endl;
    delay.ms(2000);
    lcd << TFT_LCD::clear;
}

void Loop(void)
{
    
}
```

### （八）内部Flash的读写
引入头文件
```cpp
#include "OnChip/FLASH/FLASH.hpp"
```
编写写入变量并读出的例程：
```cpp
    uint8_t a;
    uint16_t b;
    uint32_t c;

    //擦除扇区
    Platform_Flash_EraseCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0));
    a = 15;
    b = 18;
    c = 19;
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0), (uint8_t *)&a, sizeof (a)); //每次写入32位数据，不满32位按0补齐
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(1), (uint8_t *)&b, sizeof (b));
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(2), (uint8_t *)&c, sizeof (c));
    cout << (uint8_t)*(CPUFLASH_ADDRESS_32B(0))  << '\n';
    cout << (uint16_t)*(CPUFLASH_ADDRESS_32B(1)) << '\n';
    cout << (uint32_t)*(CPUFLASH_ADDRESS_32B(2)) << '\n';
```

编写写入结构体并读出的例程：
```cpp
    struct D{
        int a = 9;
        int b = 16;
    }d;
    D* pd;
    Platform_Flash_EraseCpuFlash(ADDR_FLASH_SECTOR_3_BANK2);
    Platform_FLash_WriteCpuFlash(ADDR_FLASH_SECTOR_3_BANK2, (uint8_t *)&d, sizeof (d));
    pd = (D *)ADDR_FLASH_SECTOR_3_BANK2;
    cout << "d.a = " << pd->a << " d.b = " << pd->b << '\n';
```

### （九）跳进系统BootLoader
用按键控制跳进系统BootLoader，按键如之前例程配置
```cpp
// 循环中需要执行的任务
static void Task_Key(void);

 void Init(void)
{
    timer.RegisterCallback(KeyScan10ms).Start(100);
}
void Loop(void)
{
    Task_Key();
}
static void Task_Key(void)
{
    static Key_TypeDef key_value;
    key_value = Key_GetValue();
    switch (key_value) {
        case Key1_Down:{
            led.Yellow();
            JumpToBootloader();
        }break;
        case Key1_Long:{
            led.Cyan();
        }break;
        default:
            break;
    }
}
```

### （十）执行复数FFT
- 不限点数CFFT
使用方法：需修改Platform/Platform-lib/DSP/FFT/InfinitePoint/FFTInc.h中的宏MAX_FFT_N进行点数修改
```cpp
#ifndef   MAX_FFT_N
#define   MAX_FFT_N		 (8192)
#endif 
```
`Main`函数中执行代码：
```cpp
static void FFT_Test(void)
{
     uint32_t i = 0;
    struct  compx s[MAX_FFT_N];
    for(i=0; i<MAX_FFT_N; i++)
    {
        /* 波形是由直流分量，500Hz正弦波组成，波形采样率MAX_FFT_N，初始相位60° */
        s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);
        s[i].imag = 0;
    }
    CFFT_InfinitePoint(s);
    FFT_GetPhaseRadians_f32(s, 0.5f);
    for (i = 0; i < MAX_FFT_N/2; i++)
    {
        cout << i << "," << s[i].real << '\n';
    }
}
```

- 固定点数的ARM-DSP库的CFFT
```cpp
static void FFT_Test(void)
{
#define TEST_LENGTH_SAMPLES 1024
    float32_t testOutput_f32[TEST_LENGTH_SAMPLES*2];
    float32_t testInput_f32[TEST_LENGTH_SAMPLES*2];
    float32_t Phase_f32[TEST_LENGTH_SAMPLES*2]; /* 相位*/
     uint32_t i = 0;

    for(i=0; i<TEST_LENGTH_SAMPLES; i++)
    {
        /* 波形是由直流分量，50Hz 正弦波组成，波形采样率 1024，初始相位 60° */
        testInput_f32[i*2] = 1 + cos(2*3.1415926f*50*i/1024 + 3.1415926f/3);
        testInput_f32[i*2+1] = 0;
    }

    CFFT_ARM_f32(testInput_f32, TEST_LENGTH_SAMPLES);;
    FFT_GetMagnitude_f32(testInput_f32, TEST_LENGTH_SAMPLES, testOutput_f32);
    FFT_GetPhaseRadians_f32(testInput_f32, TEST_LENGTH_SAMPLES, Phase_f32, 0.5f);
    for (i = 0; i < TEST_LENGTH_SAMPLES/2; i++)
    {
        cout << i << "," << testOutput_f32[i] << "," << Phase_f32[i]  << '\n';
    }
}

```
