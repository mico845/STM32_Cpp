//
// Created by Whisky on 1/14/2024.
//

#ifndef HELLOWORLD_PLATFORM_OPERATION_STM32_FLASH_H
#define HELLOWORLD_PLATFORM_OPERATION_STM32_FLASH_H
#include "Platform-STM32.h"

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
#define FLASH_FLAG_RDPERR_BANK1            FLASH_SR_RDPERR          /*!< Read Protection Error on Bank 1 flag */
#endif
#ifndef FLASH_FLAG_RDSERR_BANK1
#define FLASH_FLAG_RDSERR_BANK1            FLASH_SR_RDSERR          /*!< Read Secured Error on Bank 1 flag */
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
#define FLASH_FLAG_CRCRDERR_BANK1          FLASH_SR_CRCRDERR        /*!< CRC Read error on Bank 1 flag */
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
#define FLASH_FLAG_RDPERR_BANK2            (FLASH_SR_RDPERR   | 0x80000000U)        /*!< Read Protection Error on Bank 2 flag */
#endif
#ifndef FLASH_FLAG_RDSERR_BANK2
#define FLASH_FLAG_RDSERR_BANK2            (FLASH_SR_RDSERR   | 0x80000000U)        /*!< Read Secured Error on Bank 2 flag */
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
#define FLASH_FLAG_CRCRDERR_BANK2          (FLASH_SR_CRCRDERR | 0x80000000U)        /*!< CRC Read error on Bank 2 flag */
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


bool Platform_Flash_Lock(void);

bool Platform_Flash_Unlock(void);

/*
 * TypeProgram:
 *  FLASH_TYPEPROGRAM_FLASHWORD，用于芯片内部 Flash 编程。
    FLASH_TYPEPROGRAM_OTPWORD，用于芯片内部 OTP 存储区编程，当前的 H743 并没有这个区域，所以可以忽略。
 * FlashAddress:
 *  要编程的 Flash 地址
 * DataAddress:
 *  要编程到 Flash 的数据地址
 * */
bool Platform_Flash_Program(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress);

/*
 * Banks:
 *  擦除的 Bank 编号
 *  FLASH_BANK_1
 *  FLASH_BANK_2
 * Sector:
 *  擦除的 sector 号
 * NbSectors:
 *  擦除的 sector 数量
 * */
bool Platform_Flash_EraseSector(uint32_t Banks, uint32_t Sector, uint32_t NbSectors);

#endif //HELLOWORLD_PLATFORM_OPERATION_STM32_FLASH_H
