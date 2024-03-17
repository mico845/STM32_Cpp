#ifndef __PLATFORM_STM32_ADC_H__
#define __PLATFORM_STM32_ADC_H__

#include "platform-drivers/STM32/PWM/PWM.hpp"
#include "platform-lib/DataStructre/Fifo.hpp"
#include "platform-drivers/STM32/DMA/DMA.hpp"

#define USE_IRQ_FOR_DATA_HANDLING       (0)         /**< 在中断服务函数里进行数据搬运（双缓冲） */
#define USE_POLLFOR_FOR_DATA_HANDLING   (1)         /**< 在CPU循环函数中轮询扫描进行数据搬运（双缓冲） */
#define USE_YOURSELF_DMAIRQ             (0)         /**< 使用自定义的DMA中断处理 */

namespace STM32
{

#ifndef ADC_BUFFER_SIZE
    #define ADC_BUFFER_SIZE        (1024)                    /**< ADC的缓存区FIFO大小 */
#endif

#ifndef ADC_DMA_BUFFER_SIZE
    #define ADC_DMA_BUFFER_SIZE    (ADC_BUFFER_SIZE)        /**< ADC的DMA缓存区大小 */
#endif

    /**
     * @brief ADC校准模式
     *
     * ADC的校准模式。
     */
    typedef enum
    {
        offset              =   LL_ADC_CALIB_OFFSET,          /**< 偏移校准 */
        linearity           =   LL_ADC_CALIB_LINEARITY,       /**< 线性校准 */
        offset_linearity    =   LL_ADC_CALIB_OFFSET_LINEARITY /**< 偏移和线性校准 */
    }ADC_CalibrationMode;

    /**
     * @brief ADC单端/差分模式
     *
     * ADC的单端或差分模式。
     */
    typedef enum
    {
        single       = LL_ADC_SINGLE_ENDED,                    /**< 单端模式 */
        differential = LL_ADC_DIFFERENTIAL_ENDED,              /**< 差分模式 */
        botn_single_diff = LL_ADC_BOTH_SINGLE_DIFF_ENDED       /**< 单端和差分模式 */
    }ADC_SingleDiff;

/**
 * @brief ADC类
 *
 * 用于控制ADC（模数转换器）。
 */
class ADC
{
private:
    /* Variables */
    ADC_TypeDef* adc;
    PWM timer;
    bool isUseDma;
    bool isUseTimer;
    DMA_TypeDef* adc_dma;
    DMA_Stream adc_dma_stream;
    uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
    Fifo<uint16_t, ADC_BUFFER_SIZE> bufferADC;
    bool flag_finished = false;
    bool flag_halfFinished = false;
public:
    /* Variables */

    /* Constructor */
    ADC() = default;
    ~ADC() = default;

    /**
     * @brief ADC类构造函数
     *
     * 不使用DMA和定时器。
     *
     * @param ADCx ADC指针
     * @param CalibrationMode 校准模式，默认为offset
     * @param SingleDiff 单端/差分模式，默认为single
     */
    ADC(ADC_TypeDef* ADCx,
        ADC_CalibrationMode CalibrationMode = offset,
        ADC_SingleDiff SingleDiff = single);

    /**
     * @brief ADC类构造函数
     *
     * 使用DMA和定时器（TIM溢满中断）。
     *
     * @param ADCx ADC指针
     * @param ADC_DMA DMA指针
     * @param ADC_DMA_Stream DMA流
     * @param TIMx 定时器指针
     * @param CalibrationMode 校准模式，默认为offset
     * @param SingleDiff 单端/差分模式，默认为single
     */
    ADC(ADC_TypeDef* ADCx, DMA_TypeDef* ADC_DMA, DMA_Stream ADC_DMA_Stream, TIM_TypeDef *TIMx,
        ADC_CalibrationMode CalibrationMode = offset,
        ADC_SingleDiff SingleDiff = single);

    /**
     * @brief ADC类构造函数
     *
     * 使用DMA和定时器（TIM比较中断）。
     *
     * @param ADCx ADC指针
     * @param ADC_DMA DMA指针
     * @param ADC_DMA_Stream DMA流
     * @param PWMx 定时器指针
     * @param PWM_Channel 定时器比较通道
     * @param CalibrationMode 校准模式，默认为offset
     * @param SingleDiff 单端/差分模式，默认为single
     */
    ADC(ADC_TypeDef* ADCx, DMA_TypeDef* ADC_DMA, DMA_Stream ADC_DMA_Stream, TIM_TypeDef *PWMx, STM32::Timer_Channel PWM_Channel,
        ADC_CalibrationMode CalibrationMode = offset,
        ADC_SingleDiff SingleDiff = single);

    /* Functions */
    /**
     * @brief 对ADC进行校准
     *
     * @param CalibrationMode ADC校准模式
     * @param SingleDiff ADC单端/差分模式
     * @return ADC& 返回ADC对象的引用
     */
    ADC& calibrate(ADC_CalibrationMode CalibrationMode, ADC_SingleDiff SingleDiff);
    /**
     * @brief 启动ADC转换
     *
     * @param SampleRate 采样率（单位：Hz）
     * @return ADC& 返回ADC对象的引用
     */
    ADC& start(uint32_t SampleRate);
    /**
     * @brief 停止ADC转换
     *
     * @return ADC& 返回ADC对象的引用
     */
    ADC& stop(void);
    /**
     * @brief 从ADC获取数据（轮询方式）
     *
     * 支持无中断和无DMA时使用。
     *
     * @return uint16_t 返回ADC转换的数据
     */
    uint16_t getData_Pollfor(void);

    /**
     * @brief DMA中断服务函数
     *
     */
    virtual void irq_DMA(void);
    /**
     * @brief 检查ADC转换是否完成
     *
     * @return true 转换已完成
     * @return false 转换尚未完成
     */
    bool isFinished(void);
    /**
     * @brief 从ADC缓冲区中获取数据
     *
     * @param buffer 存储获取的数据的缓冲区
     * @param number 要获取的数据量
     * @return true 成功获取了数据
     * @return false 缓冲区中的数据不足
     */
    bool getData(uint16_t *buffer, uint32_t number);
    /**
     * @brief 获取ADC缓冲区中当前可用数据的数量
     *
     * @return uint32_t 当前可用数据的数量
     */
    uint32_t getDataSize(void);

#if USE_POLLFOR_FOR_DATA_HANDLING
    /**
     * @brief 扫描ADC缓冲区以获取数据
     *
     *  一般放在循环函数中轮询扫描。
     *
     * @return true 成功获取了数据
     * @return false 缓冲区中的数据不足
     */
    bool scanData(void);
#endif

};
    
} //namespace STM32
#endif //__PLATFORM_STM32_ADC_H__
