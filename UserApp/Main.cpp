//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

// 循环中需要执行的任务
static void Task_Key(void);
static void Task_Led_Flash(void);
static void Task_ADC_Finished(void);
static void Task_UART_Finished(void);

//初始化需要执行的任务
static void DAC_WaveStart(void);

void Init(void)
{
    led.PinkishRed();
    cout << "Hello World" << '\n';
    timer.RegisterCallback(KeyScan10ms).Start(100);
    // adc.Start(1000);
    // cout.Receive_Start_DMA();
    DAC_WaveStart();
    cout << "Config Finished\n";
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

static void DAC_WaveStart(void)
{
    uint8_t i;
    uint16_t g_usWaveBuff[64];
    for(i =0; i < 32; i++)
    {
        g_usWaveBuff[i] = 0;
    }

    for(i =0; i < 32; i++)
    {
        g_usWaveBuff[i+32] = 4095;
    }
    dac.Set_DMABuffer(g_usWaveBuff, 64);
    dac.Start(1000);
}