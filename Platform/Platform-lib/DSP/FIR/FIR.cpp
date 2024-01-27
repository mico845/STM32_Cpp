//
// Created by Whisky on 1/16/2024.
//

#include "FIR.hpp"

arm_fir_instance_f32 fir_instance_f32;
uint32_t blockSize = BLOCK_SIZE;
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* 状态缓存，大小numTaps + blockSize - 1*/

void FIR_Init(void)
{
    arm_fir_init_f32(&fir_instance_f32,
                     NUM_TAPS,
                     (float32_t *)&firCoeffs32[0],
                     &firStateF32[0],
                     blockSize);

}

void FIR_Run(float32_t *inputBuffer, uint32_t FIRPoints, float32_t *outputBuffer)
{
    uint32_t i;
    uint32_t numBlocks = FIRPoints/BLOCK_SIZE;
    /* 实现FIR滤波，这里每次处理1个点 */
    for(i=0; i < numBlocks; i++)
        arm_fir_f32(&fir_instance_f32, inputBuffer + (i * blockSize), outputBuffer + (i * blockSize), blockSize);
}
