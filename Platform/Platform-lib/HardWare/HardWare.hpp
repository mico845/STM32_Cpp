#ifndef __PLATFORM_HARDWARE_H__
#define __PLATFORM_HARDWARE_H__

#define HARDWARE_USE_AD9959 0
#define HARDWARE_USE_ADS1256 1

#include "platform-lib/HardWare/Key/Key.hpp"
#include "platform-lib/HardWare/LED_RGB/LED_RGB.hpp"
#include "platform-lib/HardWare/LCD_SPI/LCD_SPI.hpp"



#if HARDWARE_USE_AD9959
#include "platform-lib/HardWare/AD9959/AD9959.hpp"
#endif

#if HARDWARE_USE_ADS1256
#include "platform-lib/HardWare/ADS1256/ADS1256.hpp"
#endif

#endif // !__PLATFORM_HARDWARE_H__