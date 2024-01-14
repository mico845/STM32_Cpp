//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"
#include "OnChip/FLASH/FLASH.hpp"

using namespace Platform::HardWare;

// 循环中需要执行的任务
static void Task_Key(void);
static void Task_Led_Flash(void);
static void Task_ADC_Finished(void);
static void Task_UART_Finished(void);

//需要执行的任务
static void DDS_WaveStart(void);
static void Flash_Test(void);

 void Init(void)
{
    led.PinkishRed();
    cout << "Hello World" << '\n';
    Flash_Test();
    timer.RegisterCallback(KeyScan10ms).Start(100);
    // adc.Start(1000);
    // cout.Receive_Start_DMA();
    // DDS_WaveStart();
}

void Loop(void)
{
    Task_Key();
    Task_Led_Flash();
    Task_ADC_Finished();
    Task_UART_Finished();
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

static void Task_UART_Finished(void)
{
    static uint8_t buffer[256] = {0};
    if (cout.Is_ReceivedFinished())
    {
        cout.GetReceivedData((uint8_t *)buffer, cout.Get_ReceivedSize());
        cout << (char *)buffer << '\n';
    }
}

static void DDS_WaveStart(void)
{
    #define DDS_ROM_WAVE_SIZE (4096 * 16)
    static uint16_t SinWave_ROM[DDS_ROM_WAVE_SIZE];
    generateSineWaveTable(SinWave_ROM, DDS_ROM_WAVE_SIZE, 3);
    DDS_Config(dds, 4096000,SinWave_ROM, DDS_ROM_WAVE_SIZE, 40000, 0).Start();
}

static void Flash_Test(void)
{
#if 0   //读写变量
    uint8_t a;
    uint16_t b;
    uint32_t c;

    //擦除扇区
    Platform_Flash_EraseCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0));
    a = 15;
    b = 18;
    c = 19;
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0), (uint8_t *)&a, sizeof (a));
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(1), (uint8_t *)&b, sizeof (b));
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(2), (uint8_t *)&c, sizeof (c));
    cout << (uint8_t)*(CPUFLASH_ADDRESS_32B(0))  << '\n';
    cout << (uint16_t)*(CPUFLASH_ADDRESS_32B(1)) << '\n';
    cout << (uint32_t)*(CPUFLASH_ADDRESS_32B(2)) << '\n';
#else        //读写结构体
    struct D{
     int a = 9;
     int b = 16;
    }d;
    D* pd;
    Platform_Flash_EraseCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0));
    Platform_FLash_WriteCpuFlash((uint32_t)CPUFLASH_ADDRESS_32B(0), (uint8_t *)&d, sizeof (d));
    pd = (D *)CPUFLASH_ADDRESS_32B(0);
    cout << "d.a = " << pd->a << " d.b = " << pd->b << '\n';
#endif
}