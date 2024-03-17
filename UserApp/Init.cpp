#include "Machine.hpp"

using namespace STM32;

/**< 定义全局对象 */
ALIGN_32B(ADC             adc   __AT_SRAM4_);
ALIGN_32B(DAC             dac   __AT_SRAM4_ );
DDS             dds;
