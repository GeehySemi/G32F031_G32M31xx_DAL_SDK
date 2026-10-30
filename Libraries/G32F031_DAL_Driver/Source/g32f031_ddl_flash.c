/**
  *
  * @file    g32f031_ddl_flash.c
  * @brief   FLASH DDL module driver.
  * @version V1.0.1
  * @date    2026-09-30
  *
  * @attention
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *
  * 1. Redistributions of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of the copyright holder nor the names of its contributors
  *    may be used to endorse or promote products derived from this software without
  *    specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  * OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * The original code has been modified by Geehy Semiconductor.
  *
  * Copyright (c) 2016 STMicroelectronics.
 *  Copyright (C) 2026 Geehy Semiconductor
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  */
#if defined(USE_FULL_DDL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "g32f031_ddl_flash.h"
#include "g32f031_ddl_rcc.h"

#ifdef  USE_FULL_ASSERT
#include "g32_assert.h"
#else
#define ASSERT_PARAM(_PARAM_) ((void)0U)
#endif /* USE_FULL_ASSERT */

/** @addtogroup G32F031_DDL_Driver
  * @{
  */

#if defined (FLASH)

/** @addtogroup FLASH_DDL FLASH
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static ErrorStatus FLASH_WaitNotBusy(uint32_t Timeout);

/**
  * @brief  Wait until BUSYFLG clears or OPENDFLG sets, with timeout.
  */
static ErrorStatus FLASH_WaitNotBusy(uint32_t Timeout)
{
  while (DDL_FLASH_IsActiveFlag_BUSY() != 0U)
  {
    if (DDL_FLASH_IsActiveFlag_OPEND() != 0U)
    {
      break;
    }
    if (Timeout == 0U)
    {
      return ERROR;
    }
    Timeout--;
  }

  return SUCCESS;
}

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_DDL_Exported_Functions FLASH Exported Functions
  * @{
  */

/** @addtogroup FLASH_DDL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize FLASH registers (Registers restored to their default values).
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FLASH registers are de-initialized
  *          - ERROR: FLASH registers are not de-initialized
  */
ErrorStatus DDL_FLASH_DeInit(void)
{
  ErrorStatus status = SUCCESS;

  DDL_FLASH_REG_Unlock();
  FLASH->CR = 0x00000400U;
  FLASH->IER = 0x00000000U;
  FLASH->SR = 0x00000000U;
  FLASH->CR1 = 0x00000003U;
  DDL_FLASH_REG_Lock();

  return (status);
}

/**
  * @brief  Flash data write.
  * @param  Address The address of data to download (must be 4-byte aligned)
  * @param  Data The data to download
  * @note   32-bit access only. Caller must unlock WKEY and the matching region key first
  *         (MAIN / NVRC / NVRD). Do not program below 1.8 V. Do not change OPERATE while BUSY.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FLASH word is programmed
  *          - ERROR: address not aligned, timeout or program error
  */
ErrorStatus DDL_FLASH_Program(uint32_t Address, uint32_t Data)
{
  ErrorStatus status = SUCCESS;

  if ((Address & 0x3U) != 0U)
  {
    return ERROR;
  }

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_VALUE);
  if (status != SUCCESS)
  {
    return ERROR;
  }

  DDL_FLASH_ClearFlag_PGERR();

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);

  *(__IO uint32_t*)(Address) = Data;

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_PROGRAM);

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_READ);

  if (DDL_FLASH_IsActiveFlag_OPEND())
  {
    DDL_FLASH_ClearFlag_OPEND();
  }

  if (DDL_FLASH_IsActiveFlag_RPTERR() || DDL_FLASH_IsActiveFlag_KEYERR())
  {
    DDL_FLASH_ClearFlag_PGERR();
    status = ERROR;
  }

  return (status);
}

/**
  * @brief  Flash erase chip.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FLASH chip is erased
  *          - ERROR: timeout or erase error
  */
ErrorStatus DDL_FLASH_EraseChip(void)
{
  ErrorStatus status = SUCCESS;

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_VALUE);
  if (status != SUCCESS)
  {
    return ERROR;
  }

  DDL_FLASH_ClearFlag_PGERR();

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_CHIPERASE);

  *(__IO uint32_t *)(FLASH_BASE) = DDL_FLASH_DUMMY_OPERATE;

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_ERASE_CHIP);

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_READ);

  if (DDL_FLASH_IsActiveFlag_OPEND())
  {
    DDL_FLASH_ClearFlag_OPEND();
  }

  if (DDL_FLASH_IsActiveFlag_RPTERR() || DDL_FLASH_IsActiveFlag_KEYERR())
  {
    DDL_FLASH_ClearFlag_PGERR();
    status = ERROR;
  }

  return (status);
}


/**
  * @brief  Flash erase sector.
  * @param  Address Any address in the target sector (no 4-byte align required)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FLASH sector is erased
  *          - ERROR: timeout or erase error
  */
ErrorStatus DDL_FLASH_EraseSector(uint32_t Address)
{
  ErrorStatus status = SUCCESS;

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_VALUE);
  if (status != SUCCESS)
  {
    return ERROR;
  }

  DDL_FLASH_ClearFlag_PGERR();

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_SECTORERASE);

  *(__IO uint32_t *)(Address) = DDL_FLASH_DUMMY_OPERATE;

  status = FLASH_WaitNotBusy(DDL_FLASH_TIMEOUT_ERASE_SECTOR);

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_READ);

  if (DDL_FLASH_IsActiveFlag_OPEND())
  {
    DDL_FLASH_ClearFlag_OPEND();
  }

  if (DDL_FLASH_IsActiveFlag_RPTERR() || DDL_FLASH_IsActiveFlag_KEYERR())
  {
    DDL_FLASH_ClearFlag_PGERR();
    status = ERROR;
  }

  return (status);
}

/**
  * @brief  Flash write.
  * @param  addr The address of data to download
  * @param  size The size of data to download
  * @param  buf  Data to download
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FLASH registers are de-initialized
  *          - ERROR: FLASH registers are not de-initialized
  */
ErrorStatus DDL_FLASH_Write(uint32_t addr, uint32_t size, uint8_t *buf)
{
  ErrorStatus status = SUCCESS;

  while(DDL_FLASH_IsActiveFlag_BUSY()){}

  DDL_FLASH_MAIN_Unlock();

  DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);

  /* word alignment */
  size &= ~3;

  while (size)
  {
    (*(uint32_t*)(addr)) = *(uint32_t*)buf;

    while (DDL_FLASH_IsActiveFlag_BUSY()){}

    if (DDL_FLASH_IsActiveFlag_PGERR())
    {
        DDL_FLASH_ClearFlag_PGERR();
        status = ERROR;
        return status;
    }

    while (!DDL_FLASH_IsActiveFlag_OPEND()) {}

    DDL_FLASH_ClearFlag_OPEND();

    addr += 4;
    buf += 4;
    size -= 4;
  }

  return (status);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (FLASH) */

/**
  * @}
  */

#endif /* USE_FULL_DDL_DRIVER */
