//
// Created by Whisky on 1/14/2024.
//

#include "Platform-Operation-STM32_FLASH.h"
#include "Platform-Operation-Lock.h"

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


