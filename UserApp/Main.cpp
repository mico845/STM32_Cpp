//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

uint16_t buffer[1024] = {0};

Key_TypeDef key_value;

void Init(void)
{
    led.PinkishRed();
    cout << "Hello World" << '\n';
    timer.RegisterCallback(KeyScan10ms).Start(100);
    delay.ms(1000);
    adc.Start(1000);
    cout << "Finished\n";
   // cout.Receive_Start_DMA();
}

uint8_t count = 0;

void Loop(void)
{
    key_value = Key_GetValue();
    if (key_value == Key1_Down)
    {
        led.Yellow();
    }
    else if (key_value == Key1_Long)
    {
        led.Cyan();
    }

    if (timer.Is_Timeout())
    {
        count++;
        if (count == 100)
        {
            led.Toggle();
            count = 0;
        }
    }

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
        delay.ms(1000);
        //adc.Start(1000);
    }
    if (cout.Is_ReceivedFinished())
    {
        cout.GetReceivedData((uint8_t *)buffer, cout.Get_ReceivedSize());
        cout << (char *)buffer << '\n';
    }
}
