#ifndef __PLATFORM_DSP_DDS_H__
#define __PLATFORM_DSP_DDS_H__
#include "platform-drivers/STM32/DAC/DAC.hpp"

namespace STM32 {
    /**
     * @brief DDS类（片上DAC实现）
     *
     * 利用片上DAC加DMA双缓冲实现DDS。
     *
     */
    class DDS {
    private:
        STM32::DAC *pdac;
        uint64_t clkFreq;
        uint32_t F_WORD = 4;    //频率控制字
        uint32_t P_WORD = 0;    //相位控制字
        uint32_t ROM_Index = 0;
        const uint16_t *pROM;
        uint32_t ROM_Num;
    public:
        /* Variables */

        /* Constructor */
        DDS() = default;

        ~DDS() = default;
        /**
         * @brief DDS类构造函数
         * @param dac DAC对象
         * @param ClkFreq DDS时钟频率（单位：Hz）
         */
        DDS(STM32::DAC *dac, uint64_t ClkFreq = 96000000)
                : pdac(dac), clkFreq(ClkFreq)
                {

                }

        /* Functions */
        /**
         * @brief 启动DDS
         * @return DDS& 返回当前DDS对象的引用
         */
        DDS &start(void);
        /**
         * @brief 停止DDS
         * @return DDS& 返回当前DDS对象的引用
         */
        DDS &stop(void);
        /**
         * @brief 设置波表
         *
         * 操作时是以指针操作，故请配置参数时将数组选择为在使用时不会解构的数组（如全局变量等）。
         *
         * @param Buffer 波表数据缓冲区指针
         * @param Num 波表大小
         * @return DDS& 返回当前DDS对象的引用
         */
        DDS &setWaveRom(const uint16_t *Buffer, uint32_t Num);
        /**
         * @brief 设置DDS时钟频率
         * @param ClkFreq DDS时钟频率（单位：Hz）
         */
        inline void setClkFreq(uint64_t ClkFreq);
        /**
         * @brief 设置频率控制字
         * @param FWORD 频率控制字
         */
        inline void setFword(uint32_t FWORD);
        /**
         * @brief 设置相位控制字
         * @param PWORD 相位控制字
         */
        inline void setPword(uint32_t PWORD);
        /**
         * @brief DMA中断服务函数
         */
        void irq_DMA(void);
    };

    inline void DDS::setFword(uint32_t FWORD) {
        F_WORD = FWORD;
    }

    inline void DDS::setPword(uint32_t PWORD) {
        P_WORD = PWORD;
    }

    inline void DDS::setClkFreq(uint64_t ClkFreq) {
        clkFreq = ClkFreq;
        pdac->setFreq(ClkFreq);
    }

} //namespace STM32

/**
 * @brief DDS配置函数
 * @param dds DDS对象
 * @param clkFreq DDS时钟频率（单位：Hz）
 * @param Buffer 波表数据指针（操作时是以指针操作，故请配置参数时将数组选择为在使用时不会解构的数组，如全局变量等）
 * @param Num 波表大小
 * @param Freq 输出波形频率（单位：Hz）
 * @param Phase 输出波形相位
 * @return DDS& 返回当前DDS对象的引用
 */
STM32::DDS& DDS_Config(STM32::DDS& dds, uint64_t clkFreq,const uint16_t* Buffer, uint32_t Num, uint32_t Freq, float Phase);

#endif //__PLATFORM_DSP_DDS_H__
