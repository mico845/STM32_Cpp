//
// Created by Whisky on 1/14/2024.
//

#ifndef HELLOWORLD_FLASH_H
#define HELLOWORLD_FLASH_H
#include "Platform-STM32.h"

#ifndef CPU_FLASH_BASE_ADDR
#define CPU_FLASH_BASE_ADDR      (uint32_t)(FLASH_BASE)     /* 0x08000000 */
#endif

#ifndef CPU_FLASH_END_ADDR
#define CPU_FLASH_END_ADDR       (uint32_t)(0x081FFFFF)
#endif

#ifndef CPU_FLASH_SIZE
#define CPU_FLASH_SIZE           (2 * 1024 * 1024)  /* Flash 总容量 H743为2MB ，两个bank，8个扇区  */
#endif

#ifndef CPU_FLASH_SECTOR_SIZE
#define CPU_FLASH_SECTOR_SIZE    (128 * 1024)       /* 扇区大小，单位：字节 */
#endif


#ifndef FLASH_SECTOR_0
#define FLASH_SECTOR_0             0U       /*!< Sector Number 0   */
#endif
#ifndef FLASH_SECTOR_1
#define FLASH_SECTOR_1             1U       /*!< Sector Number 1   */
#endif
#ifndef FLASH_SECTOR_2
#define FLASH_SECTOR_2             2U       /*!< Sector Number 2   */
#endif
#ifndef FLASH_SECTOR_3
#define FLASH_SECTOR_3             3U       /*!< Sector Number 3   */
#endif
#ifndef FLASH_SECTOR_4
#define FLASH_SECTOR_4             4U       /*!< Sector Number 4   */
#endif
#ifndef FLASH_SECTOR_5
#define FLASH_SECTOR_5             5U       /*!< Sector Number 5   */
#endif
#ifndef FLASH_SECTOR_6
#define FLASH_SECTOR_6             6U       /*!< Sector Number 6   */
#endif
#ifndef FLASH_SECTOR_7
#define FLASH_SECTOR_7             7U       /*!< Sector Number 7   */
#endif

#ifndef ADDR_FLASH_SECTOR_0_BANK1
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) /* Base @ of Sector 0, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_1_BANK1
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08020000) /* Base @ of Sector 1, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_2_BANK1
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08040000) /* Base @ of Sector 2, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_3_BANK1
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08060000) /* Base @ of Sector 3, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_4_BANK1
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08080000) /* Base @ of Sector 4, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_5_BANK1
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x080A0000) /* Base @ of Sector 5, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_6_BANK1
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x080C0000) /* Base @ of Sector 6, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_7_BANK1
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x080E0000) /* Base @ of Sector 7, 128 Kbytes */
#endif

#ifndef ADDR_FLASH_SECTOR_0_BANK2
#define ADDR_FLASH_SECTOR_0_BANK2     ((uint32_t)0x08100000) /* Base @ of Sector 0, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_1_BANK2
#define ADDR_FLASH_SECTOR_1_BANK2     ((uint32_t)0x08120000) /* Base @ of Sector 1, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_2_BANK2
#define ADDR_FLASH_SECTOR_2_BANK2     ((uint32_t)0x08140000) /* Base @ of Sector 2, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_3_BANK2
#define ADDR_FLASH_SECTOR_3_BANK2     ((uint32_t)0x08160000) /* Base @ of Sector 3, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_4_BANK2
#define ADDR_FLASH_SECTOR_4_BANK2     ((uint32_t)0x08180000) /* Base @ of Sector 4, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_5_BANK2
#define ADDR_FLASH_SECTOR_5_BANK2     ((uint32_t)0x081A0000) /* Base @ of Sector 5, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_6_BANK2
#define ADDR_FLASH_SECTOR_6_BANK2     ((uint32_t)0x081C0000) /* Base @ of Sector 6, 128 Kbytes */
#endif
#ifndef ADDR_FLASH_SECTOR_7_BANK2
#define ADDR_FLASH_SECTOR_7_BANK2     ((uint32_t)0x081E0000) /* Base @ of Sector 7, 128 Kbytes */
#endif


#define FLASH_IS_EQU		0   /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
#define FLASH_REQ_WRITE		1	/* Flash不需要擦除，直接写 */
#define FLASH_REQ_ERASE		2	/* Flash需要先擦除,再写 */
#define FLASH_PARAM_ERR		3	/* 函数参数错误 */

//支持指针访问
extern uint8_t p_CpuFlashArea[CPU_FLASH_SECTOR_SIZE];
#ifndef CPUFLASH_ADDRESS_32B
#define CPUFLASH_ADDRESS_32B(ID) (p_CpuFlashArea+32 * ID)
#endif

/*
 *  1、对于同一个地址空间，仅支持一次编程(不推荐二次编程，即使是将相应 bit 由数值 1 编程 0)。
    2、只能对已经擦除的空间做编程，擦除 1 个扇区是 128KB。
    3、H7 的 Flash 编程时，务必保证要编程的地址是 32 字节对齐的，即此地址对 32 求余为 0。并且编
    程的数据必须 32 字节整数倍。函数 bsp_WriteCpuFlash 对字节数不够 32 字节整数倍的情况自动补0。
 * */
bool Platform_Flash_EraseCpuFlash(uint32_t FlashAddress);   /* 擦除 CPU FLASH 一个扇区 */
bool Platform_FLash_WriteCpuFlash(uint32_t FlashAddress, uint8_t *Buffer, uint32_t Size);
bool Platform_FLash_ReadCpuFlash(uint32_t FlashAddress, uint8_t *Buffer, uint32_t Size);

#endif //HELLOWORLD_FLASH_H
