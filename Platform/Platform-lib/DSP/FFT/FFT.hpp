#ifndef __PLATFORM_DSP_FFT_H__
#define __PLATFORM_DSP_FFT_H__

#include <stdint.h>
#include "InfinitePoint/FFTInc.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#ifndef CFFT_DoBitReverse
    #define CFFT_DoBitReverse   (1)   /**< 设置输出位反转，CFFT_DoBitReverse=1 表示使能，CFFT_DoBitReverse=0 表示禁止。 */
#endif

/**
 * @brief 使用ARM DSP库执行单精度浮点数复数FFT
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool CFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints);
/**
 * @brief 使用ARM DSP库执行单精度浮点数IFFT
 * @param FFTBuffer 存储IFFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool CIFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints);

/**
 * @brief 使用ARM DSP库执行单精度浮点数实数FFT
 * @param InPutBuffer 输入数据的数组
 * @param FFTBuffer 输出数据的数组
 * @param FFTPoints FFT的点数
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool RFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints);
/**
 * @brief 使用ARM DSP库执行单精度浮点数实数IFFT
 * @param InPutBuffer 输入数据的数组
 * @param FFTBuffer 输出数据的数组
 * @param FFTPoints IFFT的点数
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool RIFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints);

/**
 * @brief 执行不限点数CFFT
 *
 * 带类型转换（自动将float32_t数组实数在前复数在后进行排列，用法和ARM—DSP库一致）。
 *
 * 使用时，需修改FFTInc.h中的宏MAX_FFT_N进行点数修改。
 *
 * @param FFTBuffer 存储输入和输出数据的数组
 *
 */
void CFFT_InfinitePoint(float32_t *FFTBuffer);
/**
 * @brief 执行不限点数CFFT
 *
 * 高效相对高率，无类型转换。
 *
 * 使用时，需修改FFTInc.h中的宏MAX_FFT_N进行点数修改。
 *
 * @param FFTBuffer 存储输入和输出数据的数组
 *
 */
void CFFT_InfinitePoint(struct Compx *FFTBuffer);


/**
 * @brief 获取FFT结果的幅值
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @param OutputBuffer 存储幅值结果的数组
 */
void FFT_GetMagnitude_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *OutputBuffer);
/**
 * @brief 获取FFT结果的幅值
 * @param FFTBuffer 存储FFT结果的数组
 * @param OutputBuffer 存储幅值结果的数组
 */
void FFT_GetMagnitude_f32(const struct Compx *FFTBuffer, float32_t *OutputBuffer);
/**
 * @brief 获取FFT结果的幅值
 *
 * 修改自身结构体，节约空间，存放在struct Compx的成员real中。
 *
 * @param FFTBuffer 存储FFT结果的数组
 */
void FFT_GetMagnitude_f32(struct Compx *FFTBuffer);

/**
 * @brief 获取FFT结果的相角
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @param PhaseBuffer 求出相位，单位角度制，范围(-180, 180]
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *PhaseBuffer, float32_t CmpValue);
/**
 * @brief 获取FFT结果的相角
 * @param FFTBuffer 存储FFT结果的数组
 * @param PhaseBuffer 求出相位，单位角度制，范围(-180, 180]
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(const struct Compx *FFTBuffer, float32_t *PhaseBuffer, float32_t CmpValue);
/**
 * @brief 获取FFT结果的相角
 *
 * 修改自身结构体，节约空间，存放在struct Compx的成员real中。
 *
 * @param FFTBuffer 存储FFT结果的数组
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(struct Compx *FFTBuffer, float32_t CmpValue);


/**
 * @brief 将ADC采样值转换为复数形式
 *
 * 需保证CompxBuffer数组大小是ADCPoints的两倍。
 *
 * 该函数将ADC采样结果填入数组CompxBuffer，以float32_t数组实数在前复数在后进行排列，虚部自动补0。
 *
 * @param ADCBuffer 存储ADC采样值的数组
 * @param ADCPoints ADC采样点数
 * @param CompxBuffer 存储转换后的复数数据的数组
 */
void convert_ADCValue_Compx(const uint16_t *ADCBuffer, uint32_t ADCPoints, float32_t* CompxBuffer);

#endif

#endif //__PLATFORM_DSP_FFT_H__
