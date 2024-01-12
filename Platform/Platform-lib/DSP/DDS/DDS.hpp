//
// Created by Whisky on 1/12/2024.
//

#ifndef HELLOWORLD_DDS_H
#define HELLOWORLD_DDS_H
#include "OnChip/DAC/DAC.hpp"

class DDS
{
private:
    DAC* pdac;
    uint64_t    clkFreq;
    uint32_t    F_WORD = 4;    //频率控制字
    uint32_t    P_WORD = 0;    //相位控制字
    uint32_t    ROM_Index = 0;
    const uint16_t*   pROM;
    uint32_t   ROM_Num;
public:
    /* Variables */

    /* Constructor */
    DDS() = default;
    ~DDS() = default;

    DDS(DAC* dac, uint64_t ClkFreq = 96000000)
            : pdac(dac), clkFreq(ClkFreq) {  }
    /* Functions */
    DDS& Start(void);
    DDS& Stop(void);

    DDS& Set_WaveROM(const uint16_t* Buffer, uint32_t Num);   //操作以指针操作，故请配置参数时使用全局变量
    inline void Set_ClkFreq(uint64_t ClkFreq);
    inline void Set_FWORD(uint32_t FWORD);
    inline void Set_PWORD(uint32_t PWORD);

    void Irq_DMA(void);
};

inline void DDS::Set_FWORD(uint32_t FWORD) {
    F_WORD = FWORD;
}

inline void DDS::Set_PWORD(uint32_t PWORD) {
    P_WORD = PWORD;
}
inline void DDS::Set_ClkFreq(uint64_t ClkFreq) {
    clkFreq = ClkFreq;
    pdac->dac_timer.Start(ClkFreq);
}

/* DDS相应配置函数
 * dds：     DDS类实例对象
 * clkFreq： DDS主频，即DAC定时器触发频率   Hz
 * Buffer：  波表的地址指针
 * Num：     波表的大小
 * Freq：    输出波形频率  Hz
 * Phase：   输出波形相位  度
 * */
DDS& DDS_Config(DDS& dds, uint64_t clkFreq, const uint16_t* Buffer, uint32_t Num, uint32_t Freq, float Phase);

#endif //HELLOWORLD_DDS_H
