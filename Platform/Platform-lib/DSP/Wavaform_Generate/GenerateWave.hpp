//
// Created by Whisky on 1/13/2024.
//

#ifndef HELLOWORLD_GENERATEWAVE_H
#define HELLOWORLD_GENERATEWAVE_H

#include "Platform-STM32.h"

/*
 * 正弦波生成波表
 * Buffer:      数组指针地址
 * Num:         数组个数
 * Amplitude:   0-3.3V
 * */
void generateSineWaveTable(uint16_t* Buffer, uint32_t Num, float Amplitude); //

#endif //HELLOWORLD_GENERATEWAVE_H
