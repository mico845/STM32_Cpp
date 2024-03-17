#ifndef __PLATFORM_DSP_FIR_H__
#define __PLATFORM_DSP_FIR_H__

#include "arm_math.h"
#include "FirCoefs/fdacoefs.h"

extern arm_fir_instance_f32 fir_instance_f32;

#define BLOCK_SIZE 1                /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS (NUM_TAPS)         /* 滤波器系数个数 */

/**
 * @brief 初始化FIR滤波器
 */
void FIR_Init(void);
/**
 * @brief 逐点运行FIR滤波器
 * @param inputBuffer 输入数据
 * @param outputBuffer 输出数据
 */
__STATIC_INLINE void FIR_Run_OnePoint(float32_t inputBuffer, float32_t outputBuffer);
/**
 * @brief 运行FIR滤波器
 * @param inputBuffer 输入数据缓冲区
 * @param FIRPoints FIR滤波器点数
 * @param outputBuffer 输出数据缓冲区
 */
void FIR_Run(float32_t *inputBuffer, uint32_t FIRPoints, float32_t *outputBuffer);

__STATIC_INLINE void FIR_Run_OnePoint(float32_t* inputBuffer, float32_t* outputBuffer)
{
    arm_fir_f32(&fir_instance_f32, inputBuffer, outputBuffer, 1);
}


#endif //!__PLATFORM_DSP_FIR_H__
