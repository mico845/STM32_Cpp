#include "platform-drivers/STM32/FLASH/FLASH.hpp"
#include "platform-drivers/STM32/System/Sys.hpp"
#include "platform-lib/System/Lock.hpp"
#include "string.h"

/* FLASH_Keys FLASH Keys */
#ifndef FLASH_KEY1
#define FLASH_KEY1                 0x45670123U
#endif
#ifndef FLASH_KEY2
#define FLASH_KEY2                 0xCDEF89ABU
#endif

#ifndef IS_FLASH_PROGRAM_ADDRESS_BANK1
#define IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) < FLASH_BANK2_BASE))
#endif

#ifndef IS_FLASH_PROGRAM_ADDRESS_BANK2
#define IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS) (((ADDRESS) >= FLASH_BANK2_BASE ) && ((ADDRESS) <= FLASH_END))
#endif

#if defined (FLASH_OPTCR_PG_OTP)
#ifndef IS_FLASH_PROGRAM_ADDRESS_OTP
#define IS_FLASH_PROGRAM_ADDRESS_OTP(ADDRESS)   (((ADDRESS) >= 0x08FFF000U) && ((ADDRESS) <= 0x08FFF3FFU))
#endif
#ifndef IS_FLASH_PROGRAM_ADDRESS
#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS)       (IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) || \
                                                 IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS) || \
                                                 IS_FLASH_PROGRAM_ADDRESS_OTP(ADDRESS))
#endif
#else
#ifndef IS_FLASH_PROGRAM_ADDRESS
#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS)       (IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) || \
                                                 IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS))
#endif
#endif /* FLASH_OPTCR_PG_OTP */

#ifndef FLASH_BANK_1
#define FLASH_BANK_1             0x01U                         /*!< Bank 1   */
#endif
#ifndef FLASH_BANK_2
#define FLASH_BANK_2             0x02U                         /*!< Bank 2   */
#endif
#ifndef FLASH_BANK_BOTH
#define FLASH_BANK_BOTH          (FLASH_BANK_1 | FLASH_BANK_2) /*!< Bank1 and Bank2  */
#endif

#ifndef FLASH_TYPEPROGRAM_FLASHWORD
#define FLASH_TYPEPROGRAM_FLASHWORD  0x01U        /*!< Program a flash word at a specified address */
#endif
#if defined (FLASH_OPTCR_PG_OTP)
#ifndef FLASH_TYPEPROGRAM_OTPWORD
#define FLASH_TYPEPROGRAM_OTPWORD    0x02U        /*!< Program an OTP word at a specified address  */
#endif
#endif /* FLASH_OPTCR_PG_OTP */

#ifndef FLASH_FLAG_QW_BANK1
#define FLASH_FLAG_QW_BANK1                FLASH_SR_QW              /*!< Wait Queue on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_QW_BANK2
#define FLASH_FLAG_QW_BANK2                (FLASH_SR_QW       | 0x80000000U)        /*!< Wait Queue on Bank 2 flag */
#endif

#ifndef FLASH_FLAG_BSY_BANK1
#define FLASH_FLAG_BSY_BANK1               FLASH_SR_BSY             /*!< FLASH Bank 1 Busy flag */
#endif
#ifndef FLASH_FLAG_WBNE_BANK1
#define FLASH_FLAG_WBNE_BANK1              FLASH_SR_WBNE            /*!< Write Buffer Not Empty on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_QW_BANK1
#define FLASH_FLAG_QW_BANK1                FLASH_SR_QW              /*!< Wait Queue on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_CRC_BUSY_BANK1
#define FLASH_FLAG_CRC_BUSY_BANK1          FLASH_SR_CRC_BUSY        /*!< CRC Busy on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_EOP_BANK1
#define FLASH_FLAG_EOP_BANK1               FLASH_SR_EOP             /*!< End Of Program on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_WRPERR_BANK1
#define FLASH_FLAG_WRPERR_BANK1            FLASH_SR_WRPERR          /*!< Write Protection Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_PGSERR_BANK1
#define FLASH_FLAG_PGSERR_BANK1            FLASH_SR_PGSERR          /*!< Program Sequence Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_STRBERR_BANK1
#define FLASH_FLAG_STRBERR_BANK1           FLASH_SR_STRBERR         /*!< Strobe Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_INCERR_BANK1
#define FLASH_FLAG_INCERR_BANK1            FLASH_SR_INCERR          /*!< Inconsistency Error on Bank 1 flag */
#endif
#if defined (FLASH_SR_OPERR)
#ifndef FLASH_FLAG_OPERR_BANK1
#define FLASH_FLAG_OPERR_BANK1             FLASH_SR_OPERR           /*!< Operation Error on Bank 1 flag */
#endif
#endif /* FLASH_SR_OPERR */
#ifndef FLASH_FLAG_RDPERR_BANK1
#define FLASH_FLAG_RDPERR_BANK1            FLASH_SR_RDPERR          /*!< read Protection Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_RDSERR_BANK1
#define FLASH_FLAG_RDSERR_BANK1            FLASH_SR_RDSERR          /*!< read Secured Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_SNECCERR_BANK1
#define FLASH_FLAG_SNECCERR_BANK1          FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_DBECCERR_BANK1
#define FLASH_FLAG_DBECCERR_BANK1          FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_CRCEND_BANK1
#define FLASH_FLAG_CRCEND_BANK1            FLASH_SR_CRCEND          /*!< CRC End of Calculation on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_CRCRDERR_BANK1
#define FLASH_FLAG_CRCRDERR_BANK1          FLASH_SR_CRCRDERR        /*!< CRC read error on Bank 1 flag */
#endif

#if defined (FLASH_SR_OPERR)
#ifndef FLASH_FLAG_ALL_ERRORS_BANK1
#define FLASH_FLAG_ALL_ERRORS_BANK1       (FLASH_FLAG_WRPERR_BANK1   | FLASH_FLAG_PGSERR_BANK1   | \
                                           FLASH_FLAG_STRBERR_BANK1  | FLASH_FLAG_INCERR_BANK1   | \
                                           FLASH_FLAG_OPERR_BANK1    | FLASH_FLAG_RDPERR_BANK1   | \
                                           FLASH_FLAG_RDSERR_BANK1   | FLASH_FLAG_SNECCERR_BANK1 | \
                                           FLASH_FLAG_DBECCERR_BANK1 | FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#endif
#else
#ifndef FLASH_FLAG_ALL_ERRORS_BANK1
#define FLASH_FLAG_ALL_ERRORS_BANK1       (FLASH_FLAG_WRPERR_BANK1   | FLASH_FLAG_PGSERR_BANK1   | \
                                           FLASH_FLAG_STRBERR_BANK1  | FLASH_FLAG_INCERR_BANK1   | \
                                           FLASH_FLAG_RDPERR_BANK1   | FLASH_FLAG_RDSERR_BANK1   | \
                                           FLASH_FLAG_SNECCERR_BANK1 | FLASH_FLAG_DBECCERR_BANK1 | \
                                           FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#endif
#endif /* FLASH_SR_OPERR */

#ifndef FLASH_FLAG_ALL_BANK1
#define FLASH_FLAG_ALL_BANK1              (FLASH_FLAG_BSY_BANK1      | FLASH_FLAG_WBNE_BANK1     | \
                                           FLASH_FLAG_QW_BANK1       | FLASH_FLAG_CRC_BUSY_BANK1 | \
                                           FLASH_FLAG_EOP_BANK1      | FLASH_FLAG_CRCEND_BANK1   | \
                                           FLASH_FLAG_ALL_ERRORS_BANK1) /*!< All Bank 1 flags */
#endif

#ifndef FLASH_FLAG_BSY_BANK2
#define FLASH_FLAG_BSY_BANK2               (FLASH_SR_BSY      | 0x80000000U)        /*!< FLASH Bank 2 Busy flag */
#endif
#ifndef FLASH_FLAG_WBNE_BANK2
#define FLASH_FLAG_WBNE_BANK2              (FLASH_SR_WBNE     | 0x80000000U)        /*!< Write Buffer Not Empty on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_QW_BANK2
#define FLASH_FLAG_QW_BANK2                (FLASH_SR_QW       | 0x80000000U)        /*!< Wait Queue on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_CRC_BUSY_BANK2
#define FLASH_FLAG_CRC_BUSY_BANK2          (FLASH_SR_CRC_BUSY | 0x80000000U)        /*!< CRC Busy on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_EOP_BANK2
#define FLASH_FLAG_EOP_BANK2               (FLASH_SR_EOP      | 0x80000000U)        /*!< End Of Program on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_WRPERR_BANK2
#define FLASH_FLAG_WRPERR_BANK2            (FLASH_SR_WRPERR   | 0x80000000U)        /*!< Write Protection Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_PGSERR_BANK2
#define FLASH_FLAG_PGSERR_BANK2            (FLASH_SR_PGSERR   | 0x80000000U)        /*!< Program Sequence Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_STRBERR_BANK2
#define FLASH_FLAG_STRBERR_BANK2           (FLASH_SR_STRBERR  | 0x80000000U)        /*!< Strobe Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_INCERR_BANK2
#define FLASH_FLAG_INCERR_BANK2            (FLASH_SR_INCERR   | 0x80000000U)        /*!< Inconsistency Error on Bank 2 flag */
#endif
#if defined (FLASH_SR_OPERR)
#ifndef FLASH_FLAG_OPERR_BANK2
#define FLASH_FLAG_OPERR_BANK2             (FLASH_SR_OPERR    | 0x80000000U)        /*!< Operation Error on Bank 2 flag */
#endif
#endif /* FLASH_SR_OPERR */
#ifndef FLASH_FLAG_RDPERR_BANK2
#define FLASH_FLAG_RDPERR_BANK2            (FLASH_SR_RDPERR   | 0x80000000U)        /*!< read Protection Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_RDSERR_BANK2
#define FLASH_FLAG_RDSERR_BANK2            (FLASH_SR_RDSERR   | 0x80000000U)        /*!< read Secured Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_SNECCERR_BANK2
#define FLASH_FLAG_SNECCERR_BANK2          (FLASH_SR_SNECCERR | 0x80000000U)        /*!< Single ECC Error Correction on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_DBECCERR_BANK2
#define FLASH_FLAG_DBECCERR_BANK2          (FLASH_SR_DBECCERR | 0x80000000U)        /*!< Double Detection ECC Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_CRCEND_BANK2
#define FLASH_FLAG_CRCEND_BANK2            (FLASH_SR_CRCEND   | 0x80000000U)        /*!< CRC End of Calculation on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_CRCRDERR_BANK2
#define FLASH_FLAG_CRCRDERR_BANK2          (FLASH_SR_CRCRDERR | 0x80000000U)        /*!< CRC read error on Bank 2 flag */
#endif

#if defined (FLASH_SR_OPERR)
#ifndef FLASH_FLAG_ALL_ERRORS_BANK2
#define FLASH_FLAG_ALL_ERRORS_BANK2       (FLASH_FLAG_WRPERR_BANK2   | FLASH_FLAG_PGSERR_BANK2   | \
                                           FLASH_FLAG_STRBERR_BANK2  | FLASH_FLAG_INCERR_BANK2   | \
                                           FLASH_FLAG_OPERR_BANK2    | FLASH_FLAG_RDPERR_BANK2   | \
                                           FLASH_FLAG_RDSERR_BANK2   | FLASH_FLAG_SNECCERR_BANK2 | \
                                           FLASH_FLAG_DBECCERR_BANK2 | FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#endif
#else
#ifndef FLASH_FLAG_ALL_ERRORS_BANK2
#define FLASH_FLAG_ALL_ERRORS_BANK2       (FLASH_FLAG_WRPERR_BANK2   | FLASH_FLAG_PGSERR_BANK2   | \
                                           FLASH_FLAG_STRBERR_BANK2  | FLASH_FLAG_INCERR_BANK2   | \
                                           FLASH_FLAG_RDPERR_BANK2   | FLASH_FLAG_RDSERR_BANK2   | \
                                           FLASH_FLAG_SNECCERR_BANK2 | FLASH_FLAG_DBECCERR_BANK2 | \
                                           FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#endif
#endif /* FLASH_SR_OPERR */

#ifndef FLASH_FLAG_ALL_BANK2
#define FLASH_FLAG_ALL_BANK2              (FLASH_FLAG_BSY_BANK2      | FLASH_FLAG_WBNE_BANK2     | \
                                           FLASH_FLAG_QW_BANK2       | FLASH_FLAG_CRC_BUSY_BANK2 | \
                                           FLASH_FLAG_EOP_BANK2      | FLASH_FLAG_CRCEND_BANK2   | \
                                           FLASH_FLAG_ALL_ERRORS_BANK2) /*!< All Bank 2 flags */
#endif


#ifndef IS_FLASH_FLAG_BANK1
#define IS_FLASH_FLAG_BANK1(FLAG)        (((FLAG) & FLASH_FLAG_ALL_BANK1) == (FLAG))
#endif
#if defined (DUAL_BANK)
#ifndef IS_FLASH_FLAG_BANK2
#define IS_FLASH_FLAG_BANK2(FLAG)        (((FLAG) & FLASH_FLAG_ALL_BANK2) == (FLAG))
#endif
#endif /* DUAL_BANK */

#ifndef __FLASH_GET_FLAG_BANK1
#define __FLASH_GET_FLAG_BANK1(__FLAG__)     (READ_BIT(FLASH->SR1, (__FLAG__)) == (__FLAG__))
#endif
#ifndef __FLASH_GET_FLAG_BANK2
#define __FLASH_GET_FLAG_BANK2(__FLAG__)     (READ_BIT(FLASH->SR2, ((__FLAG__) & 0x7FFFFFFFU)) == (((__FLAG__) & 0x7FFFFFFFU)))
#endif

#if defined (DUAL_BANK)
#ifndef __PLATFORM_FLASH_GET_FLAG
#define __PLATFORM_FLASH_GET_FLAG(__FLAG__)           (IS_FLASH_FLAG_BANK1(__FLAG__) ?  __FLASH_GET_FLAG_BANK1(__FLAG__) : \
                                                  __FLASH_GET_FLAG_BANK2(__FLAG__))
#endif
#else
#ifndef __PLATFORM_FLASH_GET_FLAG
#define __PLATFORM_FLASH_GET_FLAG(__FLAG__)           __HAL_FLASH_GET_FLAG_BANK1(__FLAG__)
#endif
#endif /* DUAL_BANK */

#ifndef __FLASH_CLEAR_FLAG_BANK1
#define __FLASH_CLEAR_FLAG_BANK1(__FLAG__)    WRITE_REG(FLASH->CCR1, (__FLAG__))
#endif

#ifndef __FLASH_CLEAR_FLAG_BANK2
#define __FLASH_CLEAR_FLAG_BANK2(__FLAG__)    WRITE_REG(FLASH->CCR2, ((__FLAG__) & 0x7FFFFFFFU))
#endif

#if defined (DUAL_BANK)
#ifndef __PLATFORM_FLASH_CLEAR_FLAG
#define __PLATFORM_FLASH_CLEAR_FLAG(__FLAG__)         (IS_FLASH_FLAG_BANK1(__FLAG__) ?  __FLASH_CLEAR_FLAG_BANK1(__FLAG__) : \
                                                  __FLASH_CLEAR_FLAG_BANK2(__FLAG__))
#endif
#endif /* DUAL_BANK */

#ifndef FLASH_VOLTAGE_RANGE
#define FLASH_VOLTAGE_RANGE FLASH_CR_PSIZE_1 /* 3V系统，所有选择电压范围为3——Flash program/erase by 32 bits  */
#endif


Lock_TypeDef platform_flash_lock;

bool Platform_Flash_Lock(void)
{
    /* 设置 FLASH Bank1 控制寄存器 Lock 位，即禁止访问 */
    SET_BIT(FLASH->CR1, FLASH_CR_LOCK);

    /* 验证 Flash Bank1 是否已经被锁住 */
    if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) == 0U)
        return false;
#if defined (DUAL_BANK)
    /* 设置 FLASH Bank2 控制寄存器 Lock 位，即禁止访问 */
    SET_BIT(FLASH->CR2, FLASH_CR_LOCK);

    /* 验证 Flash Bank2 是否已经被锁住 */
    if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) == 0U)
        return false;
#endif /* DUAL_BANK */
    return true;
}

bool Platform_Flash_Unlock(void)
{
    if(READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
    {
        /* 允许访问 Flash Bank1 */
        WRITE_REG(FLASH->KEYR1, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR1, FLASH_KEY2);
        /* 验证是否已经解锁 */
        if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
            return false;
    }
#if defined (DUAL_BANK)
    if(READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
    {
        /* 允许访问 Flash Bank2 */
        WRITE_REG(FLASH->KEYR2, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR2, FLASH_KEY2);
        /* 验证是否已经解锁 */
        if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
            return false;

    }
#endif /* DUAL_BANK */
    return true;
}

bool Platform_FLASH_WaitForLastOperation(uint32_t Bank)
{
    uint32_t bsyflag = FLASH_FLAG_QW_BANK1;
    uint32_t errorflag;

#if defined (DUAL_BANK)

    if (Bank == FLASH_BANK_2)
    {
        /* Select bsyflag depending on Bank */
        bsyflag = FLASH_FLAG_QW_BANK2;
    }
#endif /* DUAL_BANK */

    while(__PLATFORM_FLASH_GET_FLAG(bsyflag));

    /* Get Error Flags */
    if (Bank == FLASH_BANK_1)
    {
        errorflag = FLASH->SR1 & FLASH_FLAG_ALL_ERRORS_BANK1;
    }
#if defined (DUAL_BANK)
    else
    {
        errorflag = (FLASH->SR2 & FLASH_FLAG_ALL_ERRORS_BANK2) | 0x80000000U;
    }
#endif /* DUAL_BANK */

    /* In case of error reported in Flash SR1 or SR2 register */
    if((errorflag & 0x7FFFFFFFU) != 0U)
    {
        /* Clear error programming flags */
        __PLATFORM_FLASH_CLEAR_FLAG(errorflag);
        return false;
    }

    /* Check FLASH End of Operation flag  */
    if(Bank == FLASH_BANK_1)
    {
        if (__FLASH_GET_FLAG_BANK1(FLASH_FLAG_EOP_BANK1))
        {
            /* Clear FLASH End of Operation pending bit */
            __FLASH_CLEAR_FLAG_BANK1(FLASH_FLAG_EOP_BANK1);
        }
    }
#if defined (DUAL_BANK)
    else
    {
        if (__FLASH_GET_FLAG_BANK2(FLASH_FLAG_EOP_BANK2))
        {
            /* Clear FLASH End of Operation pending bit */
            __FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_EOP_BANK2);
        }
    }
#endif /* DUAL_BANK */

    return true;
}

bool Platform_Flash_Program(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress)
{
    bool status;
    __IO uint32_t *dest_addr = (__IO uint32_t *)FlashAddress;
    __IO uint32_t *src_addr = (__IO uint32_t*)DataAddress;
    uint32_t bank;
    uint8_t row_index = FLASH_NB_32BITWORD_IN_FLASHWORD;

    __LOCK(platform_flash_lock);

#if defined (FLASH_OPTCR_PG_OTP)
    if((IS_FLASH_PROGRAM_ADDRESS_BANK1(FlashAddress)) || (IS_FLASH_PROGRAM_ADDRESS_OTP(FlashAddress)))
#else
    if(IS_FLASH_PROGRAM_ADDRESS_BANK1(FlashAddress))
#endif /* FLASH_OPTCR_PG_OTP */
    {
        bank = FLASH_BANK_1;
    }
#if defined (DUAL_BANK)
    else if(IS_FLASH_PROGRAM_ADDRESS_BANK2(FlashAddress))
    {
        bank = FLASH_BANK_2;
    }
#endif /* DUAL_BANK */
    else
    {
        return false;
    }

    /* 等待操作完成 */
    status = Platform_FLASH_WaitForLastOperation(bank);

    if(status)
    {
        if(bank == FLASH_BANK_1)
        {
#if defined (FLASH_OPTCR_PG_OTP)
            if (TypeProgram == FLASH_TYPEPROGRAM_OTPWORD)
            {
                /* 设置 OTP 寄存器的 PG 位，使能可以编程 */
                SET_BIT(FLASH->OPTCR, FLASH_OPTCR_PG_OTP);
            }
            else
#endif
            {
                /* 设置 PG 位，使能可编程 */
                SET_BIT(FLASH->CR1, FLASH_CR_PG);
            }
        }
        else
        {
            /* 设置 PG 位 */
            SET_BIT(FLASH->CR2, FLASH_CR_PG);
        }
        __ISB();
        __DSB();
#if defined (FLASH_OPTCR_PG_OTP)
        if (TypeProgram == FLASH_TYPEPROGRAM_OTPWORD)
 {
 /* 编程 OTP(16 bits) */
 *(__IO uint16_t *)FlashAddress = *(__IO uint16_t*)DataAddress;
 }
 else
#endif
        {
            /* 对 Flash 进行编程 */
            do
            {
                *dest_addr = *src_addr;
                dest_addr++;
                src_addr++;
                row_index--;
            } while (row_index != 0U);
        }

        __ISB();
        __DSB();

        /* 等待最后一次操作完成 */
        status = Platform_FLASH_WaitForLastOperation(bank);
#if defined (DUAL_BANK)
#if defined (FLASH_OPTCR_PG_OTP)
        if (TypeProgram == FLASH_TYPEPROGRAM_OTPWORD)
 {
 /* 如果编程操作完成，关闭 OTP PG 位 */
 CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_PG_OTP);
 }
 else
#endif
        {
            if(bank == FLASH_BANK_1)
            {
                /* 如果操作完成，关闭 PG 位 */
                CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);
            }
            else
            {
                /* 如果操作完成，关闭 PG 位 */
                CLEAR_BIT(FLASH->CR2, FLASH_CR_PG);
            }
        }
#else /* Single Bank */
        #if defined (FLASH_OPTCR_PG_OTP)
    if (TypeProgram == FLASH_TYPEPROGRAM_OTPWORD)
    {
      /* If the program operation is completed, disable the OTP_PG */
      CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_PG_OTP);
    }
    else
#endif /* FLASH_OPTCR_PG_OTP */
    {
      /* If the program operation is completed, disable the PG */
      CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);
    }
#endif /* DUAL_BANK */
    }
    __UNLOCK(platform_flash_lock);
    return status;
}

void FLASH_Erase_Sector(uint32_t Sector, uint32_t Banks, uint32_t VoltageRange)
{

    if((Banks & FLASH_BANK_1) == FLASH_BANK_1)
    {
#if defined (FLASH_CR_PSIZE)
        /* Reset Program/erase VoltageRange and Sector Number for Bank1 */
        FLASH->CR1 &= ~(FLASH_CR_PSIZE | FLASH_CR_SNB);

        FLASH->CR1 |= (FLASH_CR_SER | VoltageRange | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
#else
        /* Reset Sector Number for Bank1 */
    FLASH->CR1 &= ~(FLASH_CR_SNB);

    FLASH->CR1 |= (FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
#endif /* FLASH_CR_PSIZE */
    }

#if defined (DUAL_BANK)
    if((Banks & FLASH_BANK_2) == FLASH_BANK_2)
    {
#if defined (FLASH_CR_PSIZE)
        /* Reset Program/erase VoltageRange and Sector Number for Bank2 */
        FLASH->CR2 &= ~(FLASH_CR_PSIZE | FLASH_CR_SNB);

        FLASH->CR2 |= (FLASH_CR_SER | VoltageRange  | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
#else
        /* Reset Sector Number for Bank2 */
    FLASH->CR2 &= ~(FLASH_CR_SNB);

    FLASH->CR2 |= (FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
#endif /* FLASH_CR_PSIZE */
    }
#endif /* DUAL_BANK */
}

bool Platform_Flash_EraseSector(uint32_t Banks, uint32_t Sector, uint32_t NbSectors)
{
    bool status = true;
    uint32_t sector_index;
    __LOCK(platform_flash_lock);
    /* 等待 BANK1 的操作完成 */
    if((Banks & FLASH_BANK_1) == FLASH_BANK_1)
    {
        if(!Platform_FLASH_WaitForLastOperation(FLASH_BANK_1))
        {
            status = false;
        }
    }
#if defined (DUAL_BANK)
    /* 等待 BANK2 的操作完成 */
    if((Banks & FLASH_BANK_2) == FLASH_BANK_2)
    {
        if(!Platform_FLASH_WaitForLastOperation(FLASH_BANK_1))
        {
            status = false;
        }
    }
#endif /* DUAL_BANK */

    if(status) {
        /* 扇区方式擦除 */
        for (sector_index = Sector;
             sector_index < (NbSectors + Sector); sector_index++) {
            FLASH_Erase_Sector(sector_index, Banks, FLASH_VOLTAGE_RANGE);

            if ((Banks & FLASH_BANK_1) == FLASH_BANK_1) {
                /* 等待 BANK1 操作完成 */
                status = Platform_FLASH_WaitForLastOperation(FLASH_BANK_1);

                /* 如果擦除操作完成，禁止 SER 位 */
                FLASH->CR1 &= (~(FLASH_CR_SER | FLASH_CR_SNB));
            }
#if defined (DUAL_BANK)
            if ((Banks & FLASH_BANK_2) == FLASH_BANK_2) {
                /* 等待 BANK2 操作完成 */
                status = Platform_FLASH_WaitForLastOperation(FLASH_BANK_2);

                /* 如果擦除操作完成，禁止 SER 位 */
                FLASH->CR2 &= (~(FLASH_CR_SER | FLASH_CR_SNB));
            }
#endif /* DUAL_BANK */

            if (!status) {
                /* 如果擦除出错，停止后续擦除 */
                break;
            }
        }
    }
    __UNLOCK(platform_flash_lock);
    return status;
}

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