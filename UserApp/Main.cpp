//
// Created by Whisky on 12/29/2023.
//
#include "common_inc.h"

uint8_t buffer[1024] = {0};

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

    if (adc.Is_Finished())
    {
        adc.Stop();
        cout << "adc is ok\n";
        for (int i = 0; i < ADC_DMA_BUFFER_SIZE; ++i) {
            cout << adc.Get_Data()[i] << '\n';
        }
        //delay.ms(1000);
        //adc.Start(1000);
    }
    if (cout.Is_ReceivedFinished())
    {
        cout.GetReceivedData(buffer, cout.Get_ReceivedSize());
        cout << (char *)buffer << '\n';
    }
}
