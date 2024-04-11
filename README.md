## 一、基本流程
需要添加的头文件路径以及源文件路径:
头文件路径
```
UserApp
Platform
```
源文件路径
```
"UserApp/*.*"
"Platform/platform-lib/*.*"
"Platform/platform-drivers/*.*"
```
第三方库依赖
- ARM-DSP库
    直接导入`Platform/platform-drivers/ARM_DSP/CMakeLists.txt`。

用户需要操作的文件夹为UserApp文件夹
**UserApp**
- [ `common_inc.h` ]    C和C++混编头文件
- [ `irq.cpp` ]         中断服务函数
- [ `Machine.hpp` ]     声明全局对象
- [ `Main.cpp` ]        主函数

### 一般流程 （单文件）
1. **提前根据需求在STM32CubeMX中进行硬件配置，并生成初始化代码（选择LL库）**
2. 在`Main.cpp`中声明定义，包括对对象的赋值。（一定要在函数中赋值的形式构造对象，不然容易造成操作执行顺序和硬件初始化先后混乱的现象）

例如：
```cpp
Timer timer(6, true);
void Init(void)
{
    timer.registerCallback(KeyScan10ms).start(100);
}
```
必须改为
```cpp
Timer timer;
void Init(void)
{
    timer = Timer(6, true);
    timer.registerCallback(KeyScan10ms).start(100);
}
```

完整代码即：
```cpp
#include "common_inc.h"

using namespace STM32;

Timer timer;
void Init(void)
{
    timer = Timer(6, true);
    timer.registerCallback(KeyScan10ms).start(100);
}
void Loop(void)
{

}
```
3. 为其在`irq.cpp`中添加中断服务函数。
```cpp
TIM_IRQ(6)
{
    timer.irq();
}
```

### 一般流程（多文件项目）
若需要头文件中`extern` 声明全局对象，可以例照以下方式：
1. **提前根据需求在STM32CubeMX中进行硬件配置，并生成初始化代码（选择LL库）**
2. 在`Machine.hpp`声明全局对象
```cpp
/**< 用于存放全局调用的对象 */
    extern  Timer   timer;
}
```
3. 在`Main.cpp`中定义，并在`Init`函数中赋值。（也可以在别的文件中进行类似操作）
```cpp
Timer           timer;
void Init(void)
{
    timer = Timer(6, true);
}
```
4. 如果有中断需求，在`irq.cpp`中添加中断服务函数。
```cpp
TIM_IRQ(6)
{
    timer.irq();
}
```
5. 在对应文件中引用`Machine.hpp`后使用对象`timer`。
```cpp
#include "Machine.hpp"
void Fun(void)
{
    timer.registerCallback(KeyScan10ms).start(100);
}
```
   

## 二、接口操作
见`Doc`里面的`API.html`。

## 三、使用案例
### （一）闪光灯

**Platform系统定时器修改默认配置**
库里自带一个系统定时器，默认为TIM7（需自行在STM32CubeMX打开使能）
如需更换其他定时器，可修改`Platform/platform-drivers/STM32/Timer/Timer.hpp`中的宏`SYSTIM_TIM_ID`进行切换定时器号`TIMx`。
```cpp
#ifndef SYSTIM_TIMx
    #define SYSTIM_TIM_ID               7                /**< Platform库系统定时器使用的定时器号 */
    #define _SYSTIM_TIM_LINK(AA,BB)     AA##BB
    #define __SYSTIM_TIM_LINK(AA,BB)    _SYSTIM_TIM_LINK(AA,BB)
    #define SYSTIM_TIMx                 __SYSTIM_TIM_LINK(TIM, SYSTIM_TIM_ID)
#endif // !SYSTIM_TIMx
```

**LED_RGB的GPIO设置**
可更改`Platform/platform-lib/HardWare/LED_RGB/LED_RGB.hpp`中的引脚定义
```cpp
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
```
在`Machine.hpp`中添加全局对象
```cpp
/**< 用于存放全局调用的对象 */
	extern LED_RGB led;
```

代码使用示例：
`irq.cpp`
使能需要的中断
```cpp
/* TIM */
TIM_IRQ(SYSTIM_TIM_ID)
{
    sysTimer.irq();
}
```

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_Led_Flash(void);

void Init(void)
{
    led.pinkishRed();//初始粉色
    sysTimer.registerCallback(KeyScan10ms).start(100);
}

void Loop(void)
{
    Task_Led_Flash();
}

static void Task_Led_Flash(void)
{
    static uint8_t count = 0;
    if (sysTimer.isTimeOut())
        if (count++ == 50) // 10ms * N
        {
            led.toggle();
            count = 0;
        }
}
```


### （二）按键检测

`irq.cpp`
```cpp
/* TIM */
TIM_IRQ(SYSTIM_TIM_ID)
{
    sysTimer.irq();
}

```
**按键引脚配置**
进入`Platform/platform-lib/HardWare/Key/Key.hpp`
可以更改相关宏定义：
```cpp
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
```
并在`Platform/platform-lib/HardWare/Key/Key.cpp`中的`Key_Init`添加更多的按钮
```cpp
void Key_Init(void)
{
    _key[0] = Key(KEY1_GPIO_GPIO, KEY1_GPIO_PORT, KEY1_GPIO_PIN, KEY1_GPIO_PULL);
    /* 自行添加 */
}
```
代码使用示例：
`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;
// 循环中需要执行的任务
static void Task_Key(void);

void Init(void)
{
    led.pinkishRed();//初始粉色
    sysTimer.registerCallback(KeyScan10ms).start(100); //开启一个定时10ms溢满的定时器
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
            led.yellow();
        }break;
        case Key1_Long:{
            led.cyan();
        }break;
        default:
            break;
    }
}
```

### （三）串口收发
**串口数据流修改默认配置**
修改`Platform/platform-drivers/STM32/USART/USART.hpp`中的宏`COUT_USARTx`，即可切换默认的串口输出流。
```cpp
#ifndef COUT_USARTx
    #define COUT_USART_ID               1               /**< Platform库串口输出流（cout）所使用的串口号 */
    #define _COUT_USART_LINK(AA,BB)     AA##BB
    #define __COUT_USART_LINK(AA,BB)    _COUT_USART_LINK(AA,BB)
    #define COUT_USARTx  __COUT_USART_LINK(USART, COUT_USART_ID)
#endif // !COUT_USARTx
```

`irq.cpp`
```cpp
/* USART */
USART_IRQ(COUT_USART_ID)
{
    cout.irq();
}
```
代码使用示例：

- 使用串口数据流`cout`进行数据发送

```cpp
    cout << "hello" << endl; // 发送"hello"并换行
```
- 使用DMA配置USART进行数据收发

为了方便全局调用串口对象，我们在`Machine.hpp`添加串口对象。
```cpp
/**< 用于存放全局调用的对象 */
extern STM32::USART com;
```

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

ALIGN_32B(USART com  __AT_SRAM1_); //实例化对象，存放于AT_SRAM1并32位对齐

// 循环中需要执行的任务
static void Task_UART_Finished(void);

void Init(void)
{
    com = USART(USART1,
          DMA1, DMA_Stream_1,
          DMA1,DMA_Stream_0);
    com << "Hello World!!!" << '\n';
    com.startReceive_DMA();
}

void Loop(void)
{
    Task_UART_Finished();
}

static void Task_UART_Finished(void)
{
    static uint8_t buffer[256] = {0};
    if (com.isReceivedFinished())
    {
        com.getReceivedData((uint8_t *)buffer, com.getReceivedSize());
        com << (char *)buffer << '\n';
    }
}

```
### （四）ADC采集
`Machine.hpp`
```cpp
/**< 用于存放全局调用的对象 */
extern STM32::ADC          adc;
```
`irq.cpp`
```cpp
/* ADC */
DMA_IRQ(2, 0)
{
    adc.irq_DMA();
}

```

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_ADC_Finished(void);

ALIGN_32B(ADC             adc   __AT_SRAM4_);

void Init(void)
{
    adc   = ADC(ADC3, DMA2, DMA_Stream_0, TIM1, channel1);
    adc.start(409600);
}

void Loop(void)
{
    Task_ADC_Finished();
}

static void Task_ADC_Finished(void)
{
    static uint16_t buffer[1024];
    if (adc.scanData())
    {
        adc.stop();
        if (adc.getData(buffer, adc.getDataSize()))
        {
            for (int i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
                cout << i << "," << buffer[i] << '\n';
            }
        }
        //adc.start(409600);
    }
}
```

### （五）DAC输出波形
`Machine.hpp`
```cpp
/**< 用于存放全局调用的对象 */
extern STM32::DAC          dac;
```
`irq.cpp`
```cpp
/* DAC */
DMA_IRQ(2, 1)
{
    dac.irq_DMA();
}

```

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

// 需要执行的任务
static void DAC_WaveStart(void);

ALIGN_32B(DAC             dac   __AT_SRAM4_ );

void Init(void)
{
    dac   = DAC(DAC1, 1, DMA2, DMA_Stream_1, TIM6);
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
    if (dac.setDMABuffer(SinWave_ROM, DAC_WAVE_SIZE))
        dac.start(1000 * DAC_WAVE_SIZE);
}


```

### （六）DAC模拟DDS
`Machine.hpp`
```cpp
/**< 用于存放全局调用的对象 */
extern STM32::DAC          dac;
extern STM32::DDS          dds;
```
`irq.cpp`
```cpp
/* DDS */
DMA_IRQ(2, 1)
{
    dds.irq_DMA();
}
```

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

// 需要执行的任务
static void DDS_WaveStart(void);

ALIGN_32B(ADC             adc   __AT_SRAM4_);
ALIGN_32B(DAC             dac   __AT_SRAM4_ );

DDS             dds;

void Init(void)
{
    dac   = DAC(DAC1, 1, DMA2, DMA_Stream_1, TIM6);
    dds   = DDS(&dac);
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
    DDS_Config(dds, 4096000,SinWave_ROM, DDS_ROM_WAVE_SIZE, 40000, 0).start();
}
```

### （七）SPI驱动LCD

**LCD引脚配置**
进入`Platform/platform-lib/HardWare/LCD_SPI/LCD_SPI.hpp`
可以更改相关宏定义：
```cpp
#ifndef LCD_SPI_HANDEL
    #define LCD_SPI_HANDEL  (SPI6)   /**< LCD使用的SPI指针 */
#endif

#ifndef LCD_DC_GPIO
    #define LCD_DC_GPIO
    #define LCD_DC_PORT     (GPIOB) /**< LCD_DC的GPIO端口 */
    #define LCD_DC_PIN      (4)     /**< LCD_DC的GPIO引脚号 */
#endif

#ifndef LCD_RST_GPIO
    #define LCD_RST_GPIO
    #define LCD_RST_PORT    (GPIOD) /**< LCD_RST的GPIO端口 */
    #define LCD_RST_PIN     (4)     /**< LCD_RST的GPIO引脚号 */
#endif

#ifndef LCD_PWR_GPIO
    #define LCD_PWR_GPIO
    #define LCD_PWR_PORT    (GPIOD) /**< LCD_PWR的GPIO引脚号 */
    #define LCD_PWR_PIN     (3)     /**< LCD_PWR的GPIO引脚号 */
#endif
```
代码使用示例：

`Main.cpp`
```cpp
#include "common_inc.h"

using namespace STM32;

LCD_SPI lcd;

void Init(void)
{
    lcd   = LCD_SPI(true);
    lcd.clear();
    lcd << "hello world!" << endl;
    lcd << "Ricky!" << endl;
    delay_ms(1000);
    lcd << clear;
}

void Loop(void)
{

}
```

### （八）内部Flash的读写

- 写入变量并读出的例程：
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

- 写入结构体并读出的例程：
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
#include "common_inc.h"

using namespace STM32;

// 循环中需要执行的任务
static void Task_Key(void);

void Init(void)
{
    sysTimer.registerCallback(KeyScan10ms).start(100);
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
            led.yellow();
        }break;
        case Key1_Long:{
            led.cyan();
            jumpToBootloader(); //跳进系统BootLoader
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
    struct  Compx s[MAX_FFT_N];
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

### （十一） FIR滤波
- FIR非实时滤波
```cpp
void Init(void)
{
    FIR_Init(); //FIR初始化才能使用
}

static void FIR_Test(void)
{
     uint32_t i;
    static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
    static float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 滤波后的输出 */
    for(i=0; i<TEST_LENGTH_SAMPLES; i++)
    {
/* 50Hz 正弦波+200Hz 正弦波，采样率 1KHz */
        testInput_f32_50Hz_200Hz[i] = arm_sin_f32(2*3.1415926f*50*i/TEST_LENGTH_SAMPLES) +
                                      arm_sin_f32(2*3.1415926f*200*i/TEST_LENGTH_SAMPLES);
    }
     FIR_Init();

    FIR_Run(testInput_f32_50Hz_200Hz,  TEST_LENGTH_SAMPLES, testOutput);
    for (i = 0; i < TEST_LENGTH_SAMPLES; i++)
    {
        cout << i << "," << testOutput[i] << ","<< testInput_f32_50Hz_200Hz[i]  << '\n';
    }
}
```

- 对ADC采样结果进行FIR后CFFT
```cpp
#define ADC_SAMPLERATE (4096000)
void Init(void)
{
    FIR_Init(); //FIR初始化才能使用
    adc.start(ADC_SAMPLERATE);
}
void Loop(void)
{
    Task_ADC_Finished();
}

static void Task_ADC_Finished(void)
{
    static uint16_t     adc_buf[ADC_DMA_BUFFER_SIZE];
    static float32_t    buffer[ADC_DMA_BUFFER_SIZE*2];
    static float32_t    Outbuffer[ADC_DMA_BUFFER_SIZE*2];
    float32_t* p, * outp;
    if (adc.scanData())
    {
        adc.stop();
        if (adc.getData(adc_buf, adc.getDataSize()))
        {
            convert_ADCValue_Compx(adc_buf, ADC_DMA_BUFFER_SIZE, buffer);
#if 1
            FIR_Run(buffer, ADC_DMA_BUFFER_SIZE, Outbuffer);
#else
            for (uint32_t i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
                p = buffer + i;
                outp = Outbuffer + i;
                FIR_Run_OnePoint((p), (outp));
            }
#endif
            CFFT_ARM_f32(buffer, ADC_DMA_BUFFER_SIZE);
            FFT_GetMagnitude_f32(buffer, ADC_DMA_BUFFER_SIZE/2, Outbuffer);
            for (int i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
                cout << i << "," << Outbuffer[i] << '\n';
            }
        }
        adc.start(ADC_SAMPLERATE);
    }
}
```


