#include "JumpToBootloader.hpp"
#include "platform-drivers/STM32/LL-support.h"
#include "platform-drivers/STM32/System/Sys.hpp"

void jumpToBootloader(void)
{
    uint32_t i=0;
    void (*SysMemBootJump)(void);   /* 函数指针 */
    __IO uint32_t BootAddr = SYSTEM_BOOTLOADER_ADDR; /* STM32H7的系统BootLoader地址 */
    /* 关闭所有中断 */
    DISABLE_INT();
    /* 关闭滴答定时器，复位到默认值 */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    /* 设置所有时钟到默认状态，且使用HSI时钟 */
    LL_RCC_DeInit();
    /* 关闭所有中断，清除所有中断挂起标志 */
    for (i = 0; i < 8; i++)
    {
        NVIC->ICER[i]=0xFFFFFFFF;
        NVIC->ICPR[i]=0xFFFFFFFF;
    }
    /* 使能所有中断 */
    ENABLE_INT();
    /* 用函数指针承载跳转到系统的Bootloader，首地址为MSP，地址+4是复位中断服务函数地址 */
    SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));
    /* 设置主堆栈地址 */
    __set_MSP(*(uint32_t *)BootAddr);
    /* 设置为特权级模式，使用MSP指针 */
    __set_CONTROL(0);
    /* 跳转到系统的Bootloader */
    SysMemBootJump();

    /* 如果跳转成功，将不会执行 */
    while (true);
}
