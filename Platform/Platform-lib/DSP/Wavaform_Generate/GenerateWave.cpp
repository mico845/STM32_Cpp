#include "GenerateWave.hpp"
#include <cmath>

void generateSineWaveTable(uint16_t* Buffer, uint32_t Num, float Amplitude) //正弦波生成波表代码
{
    Num++;  // Increment to include the last point
    float U = Amplitude / 2.0;

    for (uint32_t i = 0; i < Num - 1; ++i)
    {
        Buffer[i] = (uint16_t)((U*sin((1.0 * i / (Num - 1)) * 2 * 3.1415926) + U) * 4095 / 3.3)+300;
    }
}
