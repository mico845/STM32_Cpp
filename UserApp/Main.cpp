//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"
#include "OnChip/FLASH/FLASH.hpp"
#include "arm_math.h"

using namespace Platform::HardWare;

// 循环中需要执行的任务
static void Task_Key(void);
static void Task_Led_Flash(void);
static void Task_ADC_Finished(void);

//需要执行的任务
static void DDS_WaveStart(void);

 void Init(void)
{
    led.PinkishRed();
    cout << "Hello World" << '\n';
    float32_t a = arm_sin_f32( 3/2.0 * 3.1415);
    lcd << a << TFT_LCD::endl;
    timer.RegisterCallback(KeyScan10ms).Start(100);
}

void Loop(void)
{
    Task_Key();
    Task_Led_Flash();
    // Task_ADC_Finished();
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

static void DDS_WaveStart(void)
{
    #define DDS_ROM_WAVE_SIZE (4096 * 16)
    static uint16_t SinWave_ROM[DDS_ROM_WAVE_SIZE];
    generateSineWaveTable(SinWave_ROM, DDS_ROM_WAVE_SIZE, 3);
    DDS_Config(dds, 4096000,SinWave_ROM, DDS_ROM_WAVE_SIZE, 40000, 0).Start();
}