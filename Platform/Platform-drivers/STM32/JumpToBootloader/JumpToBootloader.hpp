#ifndef __PLATFORM_STM32_JUMPTOBOOTLOADER_H__
#define __PLATFORM_STM32_JUMPTOBOOTLOADER_H__

#ifndef SYSTEM_BOOTLOADER_ADDR
    #define SYSTEM_BOOTLOADER_ADDR (0x1FF09800U)
#endif

/**
 * @brief 跳转到引导加载程序
 *
 * 执行跳转到官方Bootloader程序。
 */
void jumpToBootloader(void);

#endif //__PLATFORM_STM32_JUMPTOBOOTLOADER_H__
