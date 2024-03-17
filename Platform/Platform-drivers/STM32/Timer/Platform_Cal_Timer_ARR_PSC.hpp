#ifndef __PLATFORM_CAL_TIMER_ARR_PSC_H__
#define __PLATFORM_CAL_TIMER_ARR_PSC_H__

typedef enum
{
    TIM_ARR_MAX_16b,
    TIM_ARR_MAX_32b
}PLATFORM_TIM_ARR_MXA;


static inline void Platform_Timer_Cal_ARR_PSC(uint64_t _f_in , uint32_t _f_out, uint32_t* _arr, uint16_t* _psc, PLATFORM_TIM_ARR_MXA arr_max)
{
    double Min_PSC_freq;
    double arr_f = 0, psc_f = 0;

    if (arr_max == TIM_ARR_MAX_16b)
        Min_PSC_freq = (double )_f_in / UINT16_MAX;
    else
        Min_PSC_freq = (double )_f_in / UINT32_MAX;

    if(_f_out == 0 || _f_in == 0 || _f_out > _f_in)
        return;
    for (uint16_t i = 1; i < UINT16_MAX; ++i) {
        if (_f_out > Min_PSC_freq / i) {
            psc_f = i;
            arr_f = _f_in/_f_out/i;
            break;
        }
    }
    --arr_f;--psc_f;

    *_arr = round(arr_f);
    *_psc = round(psc_f);

}

static inline PLATFORM_TIM_ARR_MXA Platform_Timer_GetARR_MAX(TIM_TypeDef *tim)
{
    if (tim == TIM2 || tim == TIM5)
        return TIM_ARR_MAX_32b;
    else
        return TIM_ARR_MAX_16b;
}

#endif //__PLATFORM_CAL_TIMER_ARR_PSC_H__
