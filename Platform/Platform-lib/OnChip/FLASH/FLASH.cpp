//
// Created by Whisky on 1/14/2024.
//

#include "FLASH.hpp"
#include "string.h"
#include "Platform-Operation/Platform-Operation-STM32_FLASH.h"

uint8_t p_CpuFlashArea[CPU_FLASH_SECTOR_SIZE] __AT_CPUFLASH_2to8_;

//比较算法，防止写重复内容
uint8_t CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize)
{
    uint32_t i;
    uint8_t ucIsEqu;	/* 相等标志 */
    uint8_t ucByte;

    /* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
    if (_ulFlashAddr + _ulSize > CPU_FLASH_BASE_ADDR + CPU_FLASH_SIZE)
    {
        return FLASH_PARAM_ERR;		/*　函数参数错误　*/
    }

    /* 长度为0时返回正确 */
    if (_ulSize == 0)
    {
        return FLASH_IS_EQU;		/* Flash内容和待写入的数据相等 */
    }

    ucIsEqu = 1;			/* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
    for (i = 0; i < _ulSize; i++)
    {
        ucByte = *(uint8_t *)_ulFlashAddr;

        if (ucByte != *_ucpBuf)
        {
            if (ucByte != 0xFF)
            {
                return FLASH_REQ_ERASE;		/* 需要擦除后再写 */
            }
            else
            {
                ucIsEqu = 0;	/* 不相等，需要写 */
            }
        }

        _ulFlashAddr++;
        _ucpBuf++;
    }

    if (ucIsEqu == 1)
    {
        return FLASH_IS_EQU;	/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
    }
    else
    {
        return FLASH_REQ_WRITE;	/* Flash不需要擦除，直接写 */
    }
}

uint32_t Platform_Flash_GetSector(uint32_t FlashAddress)
{
    uint32_t sector;

    if (((FlashAddress < ADDR_FLASH_SECTOR_1_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_0_BANK1)) || \
		((FlashAddress < ADDR_FLASH_SECTOR_1_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_0_BANK2)))
    {
        sector = FLASH_SECTOR_0;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_2_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_1_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_2_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_1_BANK2)))
    {
        sector = FLASH_SECTOR_1;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_3_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_2_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_3_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_2_BANK2)))
    {
        sector = FLASH_SECTOR_2;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_4_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_3_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_4_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_3_BANK2)))
    {
        sector = FLASH_SECTOR_3;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_5_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_4_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_5_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_4_BANK2)))
    {
        sector = FLASH_SECTOR_4;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_6_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_5_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_6_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_5_BANK2)))
    {
        sector = FLASH_SECTOR_5;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_7_BANK1) && (FlashAddress >= ADDR_FLASH_SECTOR_6_BANK1)) || \
	  ((FlashAddress < ADDR_FLASH_SECTOR_7_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_6_BANK2)))
    {
        sector = FLASH_SECTOR_6;
    }
    else if (((FlashAddress < ADDR_FLASH_SECTOR_0_BANK2) && (FlashAddress >= ADDR_FLASH_SECTOR_7_BANK1)) || \
	  ((FlashAddress < CPU_FLASH_END_ADDR) && (FlashAddress >= ADDR_FLASH_SECTOR_7_BANK2)))
    {
        sector = FLASH_SECTOR_7;
    }
    else
    {
        sector = FLASH_SECTOR_7;
    }

    return sector;
}

/* 擦除 CPU FLASH 一个扇区 */
bool Platform_Flash_EraseCpuFlash(uint32_t FlashAddress)
{
    bool status;
    uint32_t Banks;
    if (FlashAddress >= ADDR_FLASH_SECTOR_0_BANK2)
        Banks = FLASH_BANK_2;

    else
        Banks = FLASH_BANK_1;
    Platform_Flash_Unlock();
    status = Platform_Flash_EraseSector(Banks,Platform_Flash_GetSector(FlashAddress),1);
    Platform_Flash_Lock();
    return status;
}

bool Platform_FLash_WriteCpuFlash(uint32_t FlashAddress, uint8_t *Buffer, uint32_t Size)
{
    uint32_t i;
    uint8_t ucRet;

    /* 如果偏移地址超过芯片容量，则不改写输出缓存区 */
    if (FlashAddress + Size > CPU_FLASH_BASE_ADDR + CPU_FLASH_SIZE)
    {
        return false;
    }

    /* 长度为0，则默认成功*/
    if (Size == 0)
    {
        return true;
    }

    ucRet = CmpCpuFlash(FlashAddress, Buffer, Size);

    if (ucRet == FLASH_IS_EQU)
    {
        return true;
    }

    __set_PRIMASK(1);  		/* 关中断 */

    /* FLASH 解锁 */
    Platform_Flash_Unlock();

    for (i = 0; i < Size / 32; i++)
    {
        uint64_t FlashWord[4];

        memcpy((char *)FlashWord, Buffer, 32);
        Buffer += 32;

        if (Platform_Flash_Program(FLASH_TYPEPROGRAM_FLASHWORD, FlashAddress, (uint64_t)((uint32_t)FlashWord)))
        {
            FlashAddress = FlashAddress + 32; /* 递增，操作下一个256bit */
        }
        else
        {
            goto err;
        }
    }

    /* 长度不是32字节整数倍 */
    if (Size % 32)
    {
        uint64_t FlashWord[4];

        FlashWord[0] = 0;
        FlashWord[1] = 0;
        FlashWord[2] = 0;
        FlashWord[3] = 0;
        memcpy((char *)FlashWord, Buffer, Size % 32);
        if (Platform_Flash_Program(FLASH_TYPEPROGRAM_FLASHWORD, FlashAddress, (uint64_t)((uint32_t)FlashWord)))
        {
            ; // _ulFlashAddr = _ulFlashAddr + 32;
        }
        else
        {
            goto err;
        }
    }

    /* Flash 加锁， 禁止Flash控制寄存器 */
    Platform_Flash_Lock();

    __set_PRIMASK(0);  		/* 开启中断 */

    return true;

    err:
    /* Flash 加锁， 禁止Flash控制寄存器 */
    Platform_Flash_Lock();

    __set_PRIMASK(0);  		/* 开启中断 */

    return false;
}

bool Platform_FLash_ReadCpuFlash(uint32_t FlashAddress, uint8_t *Buffer, uint32_t Size)
{
    uint32_t i;

    if (FlashAddress + Size > CPU_FLASH_BASE_ADDR + CPU_FLASH_SIZE)
    {
        return false;
    }

    /* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
    if (Size == 0)
    {
        return false;
    }

    for (i = 0; i < Size; i++)
    {
        *Buffer++ = *(uint8_t *)FlashAddress++;
    }

    return true;
}