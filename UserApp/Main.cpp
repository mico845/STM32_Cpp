//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

using namespace Platform::HardWare;
#define ADC_SAMPLERATE (409600)

// 循环中需要执行的任务
static void Task_Key(void);
static void Task_Led_Flash(void);
static void Task_ADC_Finished(void);

//需要执行的任务
static void DDS_WaveStart(void);
static void DAC_WaveStart(void);
static void FFT_Test(void);

 void Init(void)
{
    led.PinkishRed();
  //  DAC_WaveStart();
    FFT_Test();
    delay.ms(1000);
  //  adc.Start(ADC_SAMPLERATE);
    timer.RegisterCallback(KeyScan10ms).Start(100);
}

void Loop(void)
{
  //  Task_Key();
    Task_Led_Flash();
 //   Task_ADC_Finished();
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
        if (adc.Get_Data(buffer, adc.Get_DataSize()))
        {
            for (int i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
                cout << i << "," << buffer[i] << '\n';
            }
        }
        adc.Start(ADC_SAMPLERATE);
    }
}

static void DAC_WaveStart(void)
{
    #define DAC_WAVE_SIZE (256)
    static uint16_t SinWave_ROM[DAC_WAVE_SIZE];
    generateSineWaveTable(SinWave_ROM, DAC_WAVE_SIZE, 3);
    if (dac.Set_DMABuffer(SinWave_ROM, DAC_WAVE_SIZE))
        dac.Start(1000 * DAC_WAVE_SIZE);
}

static void DDS_WaveStart(void)
{
  //  #define DDS_ROM_WAVE_SIZE (4096 * 16)
 //   static uint16_t SinWave_ROM[DDS_ROM_WAVE_SIZE];
  //  generateSineWaveTable(SinWave_ROM, DDS_ROM_WAVE_SIZE, 3);
  //  DDS_Config(dds, 4096000*2,SinWave_ROM, DDS_ROM_SIN_WAVE_SIZE, 40000, 0).Start();
}

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

