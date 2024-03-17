#ifndef __PLATFORM_MACHINE_H__
#define __PLATFORM_MACHINE_H__

#include "platform-drivers/STM32/Platform-STM32.hpp"
#include "platform-lib/Lib.hpp"

/**< 用于存放全局调用的对象 */
extern LED_RGB             led;
extern STM32::ADC          adc;
extern STM32::DAC          dac;
extern STM32::DDS          dds;
#endif