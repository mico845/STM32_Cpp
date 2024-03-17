#ifndef __PLATFORM_DSP_GENERATEWAVE_H__
#define __PLATFORM_DSP_GENERATEWAVE_H__

#include <stdint.h>

/**
 * @brief 生成正弦波波表
 * @param Buffer 存储波表数据的数组
 * @param Num 要生成的波表数据点数
 * @param Amplitude 正弦波幅值（单位：V） 0-3.3V
 * 正弦波生成波表代码
 */
void generateSineWaveTable(uint16_t* Buffer, uint32_t Num, float Amplitude);

#endif //__PLATFORM_DSP_GENERATEWAVE_H__
