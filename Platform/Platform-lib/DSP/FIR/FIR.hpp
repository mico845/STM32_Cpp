//
// Created by Whisky on 1/16/2024.
//

#ifndef HELLOWORLD_FIR_H
#define HELLOWORLD_FIR_H
#include "arm_math.h"
#include "FirCoefs/fdacoefs.h"

extern arm_fir_instance_f32 fir_instance_f32;

#define BLOCK_SIZE 1                /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS (NUM_TAPS)         /* 滤波器系数个数 */

void FIR_Init(void);
__STATIC_INLINE void FIR_Run_OnePoint(float32_t inputBuffer, float32_t outputBuffer);
void FIR_Run(float32_t *inputBuffer, uint32_t FIRPoints, float32_t *outputBuffer);

//逐点实时
__STATIC_INLINE void FIR_Run_OnePoint(float32_t* inputBuffer, float32_t* outputBuffer)
{
    arm_fir_f32(&fir_instance_f32, inputBuffer, outputBuffer, 1);
}

#endif //HELLOWORLD_FIR_H
