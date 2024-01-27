//
// Created by Whisky on 1/16/2024.
//

#ifndef HELLOWORLD_FFT_H
#define HELLOWORLD_FFT_H

#include "Platform-STM32.h"
#include "InfinitePoint/FFTInc.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#ifndef CFFT_DoBitReverse
#define CFFT_DoBitReverse   (1)   //设置输出位反转，bitReverseFlag=1 表示使能，bitReverseFlag=0 表示禁止。
#endif

bool CFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints);
bool CIFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints);

bool RFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints);
bool RIFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints);

/* 不限点数CFFT */
/* 需修改FFTInc.h中的宏MAX_FFT_N进行点数修改 */
void CFFT_InfinitePoint(float32_t *FFTBuffer);//带类型转换，通用ARM—DSP库
void CFFT_InfinitePoint(struct compx *FFTBuffer);//高效率，无类型转换


/* 取模函数 */
void FFT_GetMagnitude_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *OutputBuffer);
void FFT_GetMagnitude_f32(const struct compx *FFTBuffer, float32_t *OutputBuffer);
void FFT_GetMagnitude_f32(struct compx *FFTBuffer);//修改自身结构体，节约空间，存放在real中

/* 求相角函数 */
void FFT_GetPhaseRadians_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *PhaseBuffer, float32_t CmpValue);
void FFT_GetPhaseRadians_f32(const struct compx *FFTBuffer, float32_t *PhaseBuffer, float32_t CmpValue);
void FFT_GetPhaseRadians_f32(struct compx *FFTBuffer, float32_t CmpValue);//修改自身结构体，节约空间，存放在real中


/* 需保证CompxBuffer数组大小是ADCPoints的两倍 */
void ConVert_ADCValue_Compx(const uint16_t *ADCBuffer, uint32_t ADCPoints, float32_t* CompxBuffer);
#endif

#endif //HELLOWORLD_FFT_H
