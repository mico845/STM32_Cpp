#ifndef __PLATFORM_STM32_DAC_H__
#define __PLATFORM_STM32_DAC_H__

#include "platform-drivers/STM32/PWM/PWM.hpp"
#include "platform-lib/DataStructre/Array.hpp"
#include "platform-drivers/STM32/DMA/DMA.hpp"

namespace STM32{
	
#define PLATFORM_DAC_CHANNEL_NUM    (2)
        static const uint32_t MapDAC_Channel[PLATFORM_DAC_CHANNEL_NUM + 1] = {0x00, LL_DAC_CHANNEL_1, LL_DAC_CHANNEL_2};
	
#ifndef DAC_DMA_BUFFER_MAX_SIZE
    #define DAC_DMA_BUFFER_MAX_SIZE    (512)        /**< DAC的DMA缓存区最大大小 */
#endif

/**
 * @brief DAC类
 *
 * 用于控制DAC（数模转换器）。
 */
class DAC
{
public:
    friend class DDS;
private:
    /* Variables */
    DAC_TypeDef *dac;
    uint32_t channel;
    DMA_TypeDef *dma;
    DMA_Stream dma_stream;
    bool isUseDma;
    bool isUseDmaIrq;
    bool isUseTimer;
    Timer dac_timer;
    Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE> dac_dma_buffer;
public:
    /* Variables */

    /* Constructor */
    DAC() = default;
    ~DAC() = default;

    /**
     * @brief DAC类构造函数
     *
     * 不使用DMA和定时器。
     *
     * @param DACx DAC指针
     * @param DAC_Channel DAC通道号
     */
    DAC(DAC_TypeDef * DACx, uint8_t DAC_Channel)
            :   dac(DACx), channel(MapDAC_Channel[DAC_Channel]),
                isUseDma(false), isUseTimer(false) { }

    /**
     * @brief DAC类构造函数
     *
     * 使用DMA和定时器。
     *
     * @param DACx DAC指针
     * @param DAC_Channel DAC通道号
     * @param DAC_DMA DMA指针
     * @param DAC_DMA_Stream DMA流
     * @param TIMx 定时器指针
     * @param IsUseDMA_Irq 是否使用DMA中断，默认不使用
     */
    DAC(DAC_TypeDef * DACx, uint8_t DAC_Channel, DMA_TypeDef* DAC_DMA, DMA_Stream DAC_DMA_Stream, TIM_TypeDef* TIMx, bool IsUseDMA_Irq = false);

    /* Functions */
    /**
     * @brief 启动DAC转换
     *
     * @param Freq 触发频率（单位：Hz），默认为1000Hz
     * @return DAC& 返回DAC对象的引用
     */
    DAC& start(uint32_t Freq = 1000);
    /**
     * @brief 停止DAC转换
     *
     * @return DAC& 返回DAC对象的引用
     */
    DAC& stop(void);

    /**
     * @brief 设置DAC输出值
     *
     * 在数据保存寄存器中设置要加载的数据。
     *
     * @param Value 输出值
     */
    __INLINE void setValue(uint16_t Value);

    /**
     * @brief 设置DAC的DMA缓冲区
     *
     * @param Buffer 缓冲区指针
     * @param Num 缓冲区中的数据数量
     * @return true 成功设置DMA缓冲区
     * @return false 超出DMA缓冲区最大大小
     */
    bool setDMABuffer(const uint16_t* Buffer, uint32_t Num);
    /**
     * @brief 开启DMA中断
     */
    void enableDMAIrq(void);
    /**
     * @brief 关闭DMA中断
     */
    void disableDMAIrq(void);
    /**
     * @brief 允许DMA中断
     */
    __INLINE void useDMAIrq(void);
    /**
     * @brief 禁止DMA中断
     */
    __INLINE  void unuseDMAIrq(void);

    /**
     * @brief 获取DMA缓冲区的指针
     *
     * @return Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE>* DAC的缓冲区的指针
     */
    inline Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE>* getPointBufArray(void);

    /**
     * @brief DMA中断服务函数
     */
    virtual void irq_DMA(void);

    void setFreq(uint32_t Freq);
};

__INLINE void DAC::setValue(uint16_t Value) {
    LL_DAC_ConvertData12RightAligned(dac, channel, Value);  //在数据保存寄存器中设置要加载的数据
}

__INLINE void DAC::useDMAIrq(void) {
    isUseDmaIrq = true;
}

__INLINE void DAC::unuseDMAIrq(void) {
    isUseDmaIrq = false;
}

__INLINE Array<uint16_t, DAC_DMA_BUFFER_MAX_SIZE> *DAC::getPointBufArray(void) {
    return &dac_dma_buffer;
}

	
} //namespace STM32



#endif //__PLATFORM_STM32_DAC_H__
