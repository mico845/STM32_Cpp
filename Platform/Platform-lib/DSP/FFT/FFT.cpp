#include "platform-lib/DSP/FFT/FFT.hpp"
#include "arm_const_structs.h"

/**
 * @brief 获取FFT结果的幅值
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @param OutputBuffer 存储幅值结果的数组
 */
void FFT_GetMagnitude_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *OutputBuffer)
{
    arm_cmplx_mag_f32(FFTBuffer, OutputBuffer, FFTPoints);
}
/**
 * @brief 获取FFT结果的幅值
 * @param FFTBuffer 存储FFT结果的数组
 * @param OutputBuffer 存储幅值结果的数组
 */
void FFT_GetMagnitude_f32(const struct Compx *FFTBuffer, float32_t *OutputBuffer)
{
    uint32_t i;
    /* 计算幅频 */
    for(i=0; i<MAX_FFT_N; i++)
    {
        arm_sqrt_f32((float32_t)(FFTBuffer[i].real *FFTBuffer[i].real+ FFTBuffer[i].imag*FFTBuffer[i].imag ), &OutputBuffer[i]);
    }
}
/**
 * @brief 获取FFT结果的幅值
 *
 * 修改自身结构体，节约空间，存放在struct Compx的成员real中。
 *
 * @param FFTBuffer 存储FFT结果的数组
 */
void FFT_GetMagnitude_f32(struct Compx *FFTBuffer)
{
    uint32_t i;
    /* 计算幅频 */
    for(i=0; i<MAX_FFT_N; i++)
    {
        arm_sqrt_f32((float32_t)(FFTBuffer[i].real *FFTBuffer[i].real+ FFTBuffer[i].imag*FFTBuffer[i].imag ), &FFTBuffer[i].real);
    }
}

/**
 * @brief 获取FFT结果的相角
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @param PhaseBuffer 求出相位，单位角度制，范围(-180, 180]
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(const float32_t *FFTBuffer, uint32_t FFTPoints, float32_t *PhaseBuffer, float32_t CmpValue)
{
    float32_t lX, lY;
    uint16_t i;
    float32_t phase;
    float32_t mag;

    for (i=0; i < FFTPoints; i++)
    {
        lX= FFTBuffer[2 * i];  	  /* 实部 */
        lY= FFTBuffer[2 * i + 1];    /* 虚部 */

        phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
        arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */

        if(CmpValue > mag)
        {
            PhaseBuffer[i] = 0;
        }
        else
        {
            PhaseBuffer[i] = phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
        }
    }
}
/**
 * @brief 获取FFT结果的相角
 * @param FFTBuffer 存储FFT结果的数组
 * @param PhaseBuffer 求出相位，单位角度制，范围(-180, 180]
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(const struct Compx *FFTBuffer, float32_t *PhaseBuffer, float32_t CmpValue)
{
    float32_t lX, lY;
    uint32_t i;
    float32_t phase;
    float32_t mag;


    for (i=0; i <MAX_FFT_N; i++)
    {
        lX= FFTBuffer[i].real; 	  /* 实部 */
        lY= FFTBuffer[i].imag;    /* 虚部 */

        phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
        arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */

        if(CmpValue > mag)
        {
            PhaseBuffer[i] = 0;
        }
        else
        {
            PhaseBuffer[i] = phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
        }
    }
}
/**
 * @brief 获取FFT结果的相角
 *
 * 修改自身结构体，节约空间，存放在struct Compx的成员real中。
 *
 * @param FFTBuffer 存储FFT结果的数组
 * @param CmpValue 比较值，需要求出相位的数值
 */
void FFT_GetPhaseRadians_f32(struct Compx *FFTBuffer, float32_t CmpValue)
{
    float32_t lX, lY;
    uint32_t i;
    float32_t phase;
    float32_t mag;


    for (i=0; i <MAX_FFT_N; i++)
    {
        lX= FFTBuffer[i].real; 	  /* 实部 */
        lY= FFTBuffer[i].imag;    /* 虚部 */

        phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
        arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */

        if(CmpValue > mag)
        {
            FFTBuffer[i].real = 0;
        }
        else
        {
            FFTBuffer[i].real= phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
        }
    }
}

__STATIC_INLINE const arm_cfft_instance_f32* Get_CFFT_Length(uint32_t FFTPoints)
{
    if (FFTPoints == 4096)
        return &arm_cfft_sR_f32_len4096;
    else if (FFTPoints == 2048)
        return &arm_cfft_sR_f32_len2048;
    else if (FFTPoints == 1024)
        return &arm_cfft_sR_f32_len1024;
    else if (FFTPoints == 512)
        return &arm_cfft_sR_f32_len512;
    else if (FFTPoints == 256)
        return &arm_cfft_sR_f32_len256;
    else if (FFTPoints == 128)
        return &arm_cfft_sR_f32_len128;
    else if (FFTPoints == 64)
        return &arm_cfft_sR_f32_len64;
    else if (FFTPoints == 32)
        return &arm_cfft_sR_f32_len32;
    else if (FFTPoints == 16)
        return &arm_cfft_sR_f32_len16;
    else
        return nullptr;

}

__STATIC_INLINE bool __CFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints, uint32_t ifftFlag)
{
    const arm_cfft_instance_f32* cfftinstance = Get_CFFT_Length(FFTPoints);
    if (cfftinstance == nullptr)
        return false;
    arm_cfft_f32(cfftinstance, FFTBuffer, ifftFlag, CFFT_DoBitReverse);
    return true;
}

/**
 * @brief 使用ARM DSP库执行单精度浮点数复数FFT
 * @param FFTBuffer 存储FFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool CFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints)
{
    return __CFFT_ARM_f32(FFTBuffer, FFTPoints, 0);
}

/**
 * @brief 使用ARM DSP库执行单精度浮点数IFFT
 * @param FFTBuffer 存储IFFT结果的数组,含实部和虚部
 * @param FFTPoints 复数个数，每个复数是两个float32_t数值
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool CIFFT_ARM_f32(float32_t *FFTBuffer, uint32_t FFTPoints)
{
    return __CFFT_ARM_f32(FFTBuffer, FFTPoints, 1);
}

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
void CFFT_InfinitePoint(float32_t *FFTBuffer)
{
    struct  Compx s[MAX_FFT_N];
    uint32_t i;
    /* 计算一批sin，cos系数 */

#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
        InitTableFFT(MAX_FFT_N);
#endif

    for(i=0; i<MAX_FFT_N; i++)
    {
        s[i].real = FFTBuffer[i * 2];
        s[i].imag = FFTBuffer[i * 2 + 1];
    }
    cfft(s, MAX_FFT_N);
    for(i=0; i<MAX_FFT_N; i++)
    {
        FFTBuffer[i * 2] = s[i].real;
        FFTBuffer[i * 2 + 1] = s[i].imag ;
    }
}

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
void CFFT_InfinitePoint(struct Compx *FFTBuffer)
{
    /* 计算一批sin，cos系数 */

#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
        InitTableFFT(MAX_FFT_N);
#endif

    cfft(FFTBuffer, MAX_FFT_N);
}

__STATIC_INLINE bool IS_RFFT_Length(uint32_t FFTPoints)
{
    if (FFTPoints == 4096 || FFTPoints == 2048 || FFTPoints == 1024 || FFTPoints == 512 || FFTPoints == 256 ||
        FFTPoints == 128 || FFTPoints == 64 || FFTPoints == 32)
        return true;
    else
        return false;
}

__STATIC_INLINE bool __RFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer,uint32_t FFTPoints, uint32_t ifftFlag)
{
    arm_rfft_fast_instance_f32 S;
    if (!IS_RFFT_Length(FFTPoints))
        return false;
    arm_rfft_fast_init_f32(&S, FFTPoints);
    arm_rfft_fast_f32(&S, InPutBuffer, FFTBuffer, ifftFlag);
    return true;
}

/**
 * @brief 使用ARM DSP库执行单精度浮点数实数FFT
 * @param InPutBuffer 输入数据的数组
 * @param FFTBuffer 输出数据的数组
 * @param FFTPoints FFT的点数
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool RFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints)
{
   return __RFFT_ARM_f32(InPutBuffer, FFTBuffer, FFTPoints, 0);
}

/**
 * @brief 使用ARM DSP库执行单精度浮点数实数IFFT
 * @param InPutBuffer 输入数据的数组
 * @param FFTBuffer 输出数据的数组
 * @param FFTPoints IFFT的点数
 * @return true 成功变换
 * @return false 尚未变换成功
 */
bool RIFFT_ARM_f32(float32_t *InPutBuffer, float32_t *FFTBuffer, uint32_t FFTPoints)
{
    return __RFFT_ARM_f32(InPutBuffer, FFTBuffer, FFTPoints, 1);
}

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
void convert_ADCValue_Compx(const uint16_t *ADCBuffer, uint32_t ADCPoints, float32_t* CompxBuffer)
{
    for (uint32_t i= 0; i < ADCPoints; ++i) {
        CompxBuffer[2*i] = ADCBuffer[i];
        CompxBuffer[2*i+1] = 0;
    }
}
