/*!
 * @file        flash_eeprom.c
 *
 * @brief       This file contains all the functions for the eeprom emulation.
 *
 * @version     V1.0.0
 *
 * @date        2025-06-01
 *
 * @attention
 *
 *  Copyright (C) 2025 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes ***************************************************************/
#include "flash_eeprom.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Specifies the start address of the sector. The purpose is to occupy a space at the specified
 * address of MCU flash. */
#if defined(__CC_ARM)
const uint8_t
    __attribute__((section(".ARM.__at_0x00004000"))) Flash_Para_Area[FLASH_EE_PAGE_SIZE * 2];
#elif defined(__ICCARM__)
#pragma location = 0x00004000
__root const uint8_t Flash_Para_Area[FLASH_EE_PAGE_SIZE * 2];
#elif defined(__GNUC__)

#else
#warning Not supported compiler type
#endif

/* Private function prototypes ********************************************/
static uint32_t Flash_EE_FindValidPage(uint8_t mode);
static uint32_t Flash_EE_PageErase(uint32_t PageAddr);
static uint32_t Flash_EE_Format(void);
static uint16_t Flash_EE_PageStatusCheck(uint32_t PageStatus0, uint32_t PageStatus1);
static uint32_t Flash_EE_NotVerifyPageFullWriteData(uint32_t VirtAddress, uint32_t Data);
static uint32_t Flash_EE_TransferToActivePage(void);
static uint32_t Flash_EE_FullCheck(void);
static uint32_t Flash_EE_ValidTransfer(uint32_t PageStatus0, uint32_t PageStatus1);
static uint32_t Flash_EE_EraseTransfer(uint32_t PageStatus0, uint32_t PageStatus1);

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       Flash EEPROM Init. Restore the pages to a known good state in case of pages'
 *              status corruption after a power loss.
 *
 * @param       None
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
uint32_t Flash_EE_Init(void)
{
    uint32_t PageStatus0 = 0;
    uint32_t PageStatus1 = 0;
    uint32_t FlashStatus = 0;

    /* Flash unlock */
    DDL_FLASH_REG_Unlock();
    DDL_FLASH_MAIN_Unlock();

    /* Get Page0 status */
    PageStatus0 = (*(__IO uint32_t *)PAGE0_START_ADDRESS);

    /* Get Page1 status */
    PageStatus1 = (*(__IO uint32_t *)PAGE1_START_ADDRESS);

    /* Ensure that the page data is completely erased */
    if (PageStatus0 == PAGE_STATUS_ERASED)
    {
        /* Erase Page0 */
        if ((FlashStatus = Flash_EE_PageErase(PAGE0_START_ADDRESS)) != 0)
        {
            DDL_FLASH_MAIN_Lock();
            DDL_FLASH_REG_Lock();
            return FlashStatus;
        }
    }

    /* Ensure that the page data is completely erased */
    if (PageStatus1 == PAGE_STATUS_ERASED)
    {
        /* Erase Page1 */
        if ((FlashStatus = Flash_EE_PageErase(PAGE1_START_ADDRESS)) != 0)
        {
            DDL_FLASH_MAIN_Lock();
            DDL_FLASH_REG_Lock();
            return FlashStatus;
        }
    }

    /* Page0 Page1 status validity check */
    if (Flash_EE_PageStatusCheck(PageStatus0, PageStatus1) != 0)
    {
        /* If the page status is invalid, reformat */
        if ((FlashStatus = Flash_EE_Format()) != 0)
        {
            DDL_FLASH_MAIN_Lock();
            DDL_FLASH_REG_Lock();
            return FlashStatus;
        }
    }

    /* Transition state processing, one page state is valid, one page state is transfer,
       and the data is transferred to the transfer state page */
    if (((PageStatus0 == PAGE_STATUS_VALID) && (PageStatus1 == PAGE_STATUS_TRANSFER)) ||
        ((PageStatus0 == PAGE_STATUS_TRANSFER) && (PageStatus1 == PAGE_STATUS_VALID)))
    {
        if ((FlashStatus = Flash_EE_ValidTransfer(PageStatus0, PageStatus1)) != 0)
        {
            DDL_FLASH_MAIN_Lock();
            DDL_FLASH_REG_Lock();
            return FlashStatus;
        }
    }

    /* Transition state processing, a page state is erase, a page state is transfer,
       and the transfer state is changed to valid */
    if (((PageStatus0 == PAGE_STATUS_ERASED) && (PageStatus1 == PAGE_STATUS_TRANSFER)) ||
        ((PageStatus0 == PAGE_STATUS_TRANSFER) && (PageStatus1 == PAGE_STATUS_ERASED)))
    {
        if ((FlashStatus = Flash_EE_EraseTransfer(PageStatus0, PageStatus1)) != 0)
        {
            DDL_FLASH_MAIN_Lock();
            DDL_FLASH_REG_Lock();
            return FlashStatus;
        }
    }

    /* Check if the page is full, when the page is full, transfer the data to erase page */
    if ((FlashStatus = Flash_EE_FullCheck()) != 0)
    {
        DDL_FLASH_MAIN_Lock();
        DDL_FLASH_REG_Lock();
        return FlashStatus;
    }

    /* Flash lock */
    DDL_FLASH_MAIN_Lock();
    DDL_FLASH_REG_Lock();

    return 0;
}

/*!
 * @brief       Read the variable data corresponding to the last stored virtual address.
 *
 * @param       VirtAddress: the variable virtual address.
 *                           Virtual address value must be between 0 ~ NUMBER_OF_VARIABLES.
 *
 * @param       pData: data pointer.
 *
 * @retval      The returned value can be: 0 or 1.
 *              @arg 0: Read successful.
 *              @arg 1: Read fail.
 *
 * @note
 */
uint32_t Flash_EE_ReadData(uint32_t VirtAddress, uint32_t *pData)
{
    uint32_t ValidPage = 0;
    uint32_t data_address;
    uint32_t Address = 0;
    uint32_t PageStartAddress = 0;

    /* Get the valid page for read operation */
    ValidPage = Flash_EE_FindValidPage(GET_VALID_PAGE_READ);

    if (ValidPage == NO_VALID_PAGE)
    {
        return NO_VALID_PAGE;
    }

    /* Calculate the valid Page start Address */
    PageStartAddress = FLASH_EE_START_ADDRESS + ValidPage * FLASH_EE_PAGE_SIZE + 4;

    /* Calculate the valid Page end Address */
    Address = FLASH_EE_START_ADDRESS + ValidPage * FLASH_EE_PAGE_SIZE + FLASH_EE_PAGE_SIZE - 4;

    /* Search from page end address */
    while (Address > PageStartAddress)
    {
        /* Get the virtual address of the current location */
        data_address = (*(__IO uint32_t *)Address);

        /* The read address is compared with the virtual address */
        if (VirtAddress == data_address)
        {
            /* Get variable value */
            *pData = (*(__IO uint32_t *)(Address - 4));

            /* Read variable data successful, return 0 */
            return 0;
        }

        /* Next address location */
        Address -= 8;
    }

    /* Read variable data fail, return 1 */
    return 1;
}

/*!
 * @brief       Writes/upadtes variable data in Flash EEPROM.
 *
 * @param       VirtAddress: the variable virtual address.
 *                           Virtual address value must be between 0 ~ NUMBER_OF_VARIABLES.
 *
 * @param       Data: The data to be written.
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
uint32_t Flash_EE_WriteData(uint32_t VirtAddress, uint32_t Data)
{
    uint32_t FlashStatus = 0;

    /* Flash unlock */
    DDL_FLASH_REG_Unlock();
    DDL_FLASH_MAIN_Unlock();

    /* Check if the page is full, when the page is full, transfer the data to erase page */
    if ((FlashStatus = Flash_EE_FullCheck()) != 0)
    {
        DDL_FLASH_MAIN_Lock();
        DDL_FLASH_REG_Lock();
        return FlashStatus;
    }

    /* Write the data virtual address and value to flash */
    if ((FlashStatus = Flash_EE_NotVerifyPageFullWriteData(VirtAddress, Data)) != 0)
    {
        DDL_FLASH_MAIN_Lock();
        DDL_FLASH_REG_Lock();
        return FlashStatus;
    }

    /* Check if the page is full, when the page is full, transfer the data to erase page */
    if ((FlashStatus = Flash_EE_FullCheck()) != 0)
    {
        DDL_FLASH_MAIN_Lock();
        DDL_FLASH_REG_Lock();
        return FlashStatus;
    }

    /* Flash lock */
    DDL_FLASH_MAIN_Lock();
    DDL_FLASH_REG_Lock();

    return 0;
}

/* Private functions ******************************************************/

/*!
 * @brief       Get valid Page for write or read operation.
 *
 * @param       mode: Get valid pages in read and write mode.
 *                    This parameter can be any combination of the flowing values:
 *                    @arg GET_VALID_PAGE_READ
 *                    @arg GET_VALID_PAGE_WRITE
 *
 * @retval      Returns valid page number. It can be one of value:
 *                    @arg PAGE0_VALID
 *                    @arg PAGE1_VALID
 *                    @arg NO_VALID_PAGE
 *
 * @note
 */
static uint32_t Flash_EE_FindValidPage(uint8_t mode)
{
    uint32_t PageStatus0 = 0;
    uint32_t PageStatus1 = 0;

    /* Get Page0 status */
    PageStatus0 = (*(__IO uint32_t *)PAGE0_START_ADDRESS);

    /* Get Page1 status */
    PageStatus1 = (*(__IO uint32_t *)PAGE1_START_ADDRESS);

    if (mode == GET_VALID_PAGE_WRITE)
    {
        if (PageStatus0 == PAGE_STATUS_TRANSFER)
        {
            /* Page0 is valid */
            return PAGE0_VALID;
        }
        else if (PageStatus1 == PAGE_STATUS_TRANSFER)
        {
            /* Page1 is valid */
            return PAGE1_VALID;
        }
        else if (PageStatus0 == PAGE_STATUS_VALID)
        {
            /* Page0 is valid */
            return PAGE0_VALID;
        }
        else if (PageStatus1 == PAGE_STATUS_VALID)
        {
            /* Page1 is valid */
            return PAGE1_VALID;
        }
    }
    else if (mode == GET_VALID_PAGE_READ)
    {
        if (PageStatus0 == PAGE_STATUS_VALID)
        {
            /* Page0 is valid */
            return PAGE0_VALID;
        }
        else if (PageStatus1 == PAGE_STATUS_VALID)
        {
            /* Page1 is valid */
            return PAGE1_VALID;
        }
    }

    /* no valid page found */
    return NO_VALID_PAGE;
}

/*!
 * @brief       Erase flash eeprom page, can erase multiple pages.
 *
 * @param       PageAddr: Erase page base address.
 *                        This parameter can be any combination of the flowing values:
 *                        @arg PAGE0_START_ADDRESS
 *                        @arg PAGE1_START_ADDRESS
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
static uint32_t Flash_EE_PageErase(uint32_t PageAddr)
{
    uint16_t i = 0;

    /* Erase one or more flash pages */
    for (i = 0; i < FLASH_EE_PAGE_NUM; i++)
    {
        /* Calculate the erase page address */
        DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_SECTORERASE);
        *((uint32_t *)(PageAddr + i * G32_FLASH_PAGE_SIZE)) = 0xA5A5;
        while (DDL_FLASH_IsActiveFlag_BUSY())
            ;
    }

    return 0;
}

/*!
 * @brief       Erases PAGE0 and PAGE1 and writes PAGE_STATUS_VALID header to PAGE0.
 *
 * @param       None
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
static uint32_t Flash_EE_Format(void)
{
    uint32_t FlashStatus = 0;

    /* Erase Page0 */
    if ((FlashStatus = Flash_EE_PageErase(PAGE0_START_ADDRESS)) != 0)
    {
        return FlashStatus;
    }

    /* Erase Page1 */
    if ((FlashStatus = Flash_EE_PageErase(PAGE1_START_ADDRESS)) != 0)
    {
        return FlashStatus;
    }

    /* Set Page0 as valid page */
    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
    *((uint32_t *)PAGE0_START_ADDRESS) = PAGE_STATUS_VALID;
    while (DDL_FLASH_IsActiveFlag_BUSY())
        ;

    return 0;
}

/*!
 * @brief       Verify Flash EEPROM page status.
 *
 * @param       PageStatus0: Page0 status.
 *
 * @param       PageStatus1: Page1 status.
 *
 * @retval      The returned value can be: 0 or 1.
 *              @arg 0: the page status format correct.
 *              @arg 1: the page status format incorrect.
 *
 * @note
 */
static uint16_t Flash_EE_PageStatusCheck(uint32_t PageStatus0, uint32_t PageStatus1)
{
    /* the euqal status is invalid */
    if (PageStatus0 == PageStatus1)
    {
        return 1;
    }

    /* Page0 status is not known status */
    if ((PageStatus0 != PAGE_STATUS_ERASED) && (PageStatus0 != PAGE_STATUS_TRANSFER) &&
        (PageStatus0 != PAGE_STATUS_VALID))
    {
        return 1;
    }

    /* Page1 status is not known status */
    if ((PageStatus1 != PAGE_STATUS_ERASED) && (PageStatus1 != PAGE_STATUS_TRANSFER) &&
        (PageStatus1 != PAGE_STATUS_VALID))
    {
        return 1;
    }

    /* Format is correct */
    return 0;
}

/*!
 * @brief       Not verify page full, Writes variable and virtual address in Flash EEPROM.
 *
 * @param       VirtAddress: the variable virtual address.
 *                           Virtual address value must be between 0 ~ NUMBER_OF_VARIABLES.
 *
 * @param       Data: the variable value.
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
static uint32_t Flash_EE_NotVerifyPageFullWriteData(uint32_t VirtAddress, uint32_t Data)
{
    uint32_t Address = 0;
    uint32_t PageEndAddress = 0;
    uint32_t ValidPage = 0;
    uint32_t FlashStatus = 0;

    /* Get the valid page for write operation */
    ValidPage = Flash_EE_FindValidPage(GET_VALID_PAGE_WRITE);

    if (ValidPage == NO_VALID_PAGE)
    {
        return 1;
    }

    /* Calculate the valid Page start Address */
    Address = FLASH_EE_START_ADDRESS + ValidPage * FLASH_EE_PAGE_SIZE;

    /* Calculate the valid Page end Address */
    PageEndAddress =
        FLASH_EE_START_ADDRESS + ValidPage * FLASH_EE_PAGE_SIZE + FLASH_EE_PAGE_SIZE - 4;

    while (Address < PageEndAddress)
    {
        /* Verify if Address and Address+4 contents are 0xFFFFFFFF */
        if (((*(__IO uint32_t *)Address) == 0xFFFFFFFF) &&
            ((*(__IO uint32_t *)(Address + 4)) == 0xFFFFFFFF))
        {
            DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
            *((uint32_t *)Address) = Data;
            while (DDL_FLASH_IsActiveFlag_BUSY())
                ;

            /* write variable virtual address to flash */
            DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
            *((uint32_t *)(Address + 4)) = VirtAddress;
            while (DDL_FLASH_IsActiveFlag_BUSY())
                ;

            return FlashStatus;
        }
        else
        {
            /* Next address location */
            Address += 8;
        }
    }

    return 1;
}

/*!
 * @brief       Transfers valid(last updated) variables data from the full Page to an empty one.
 *
 * @param       None
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 */
static uint32_t Flash_EE_TransferToActivePage(void)
{
    uint32_t FullPageAddress = 0;
    uint32_t EmptyPageAddress = 0;
    uint32_t ReadData = 0;
    uint32_t ValidPage = 0;
    uint32_t FlashStatus = 0;

    /* This vitual address corresponds to the vitual address written by the user */
    uint32_t VirtAddr = 0;

    /* Get valid Page for read operation */
    ValidPage = Flash_EE_FindValidPage(GET_VALID_PAGE_READ);

    if (ValidPage == PAGE0_VALID)
    {
        /* Empty page is Page1 */
        EmptyPageAddress = PAGE1_START_ADDRESS;

        /* Full page is Page0 */
        FullPageAddress = PAGE0_START_ADDRESS;
    }
    else if (ValidPage == PAGE1_VALID)
    {
        /* Empty page is Page0 */
        EmptyPageAddress = PAGE0_START_ADDRESS;

        /* Full page is Page1 */
        FullPageAddress = PAGE1_START_ADDRESS;
    }
    else
    {
        return 1;
    }

    /* Set the empty Page status as transfer status */
    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
    *((uint32_t *)EmptyPageAddress) = PAGE_STATUS_TRANSFER;
    while (DDL_FLASH_IsActiveFlag_BUSY())
        ;

    /* Transfer process: transfer variables from old to the new active page */
    for (VirtAddr = 0; VirtAddr < NUMBER_OF_VARIABLES; VirtAddr++)
    {
        /* Find valid variables: the last updated variable */
        if (Flash_EE_ReadData(VirtAddr, &ReadData) == 0)
        {
            /* Transfer the variable to the new active page */
            if ((FlashStatus = Flash_EE_NotVerifyPageFullWriteData(VirtAddr, ReadData)) != 0)
            {
                return FlashStatus;
            }
        }
    }

    /* Erase the old page */
    if ((FlashStatus = Flash_EE_PageErase(FullPageAddress)) != 0)
    {
        return FlashStatus;
    }

    /* Mark empty page status as valid */
    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
    *((uint32_t *)EmptyPageAddress) = PAGE_STATUS_VALID;
    while (DDL_FLASH_IsActiveFlag_BUSY())
        ;

    return 0;
}

/*!
 * @brief       Check if the page is full. If the page is full, Transfers valid variables data to
 * empty page.
 *
 * @param       None
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 */
static uint32_t Flash_EE_FullCheck(void)
{
    uint16_t ValidPage = 0;
    uint32_t PageEndAddress = 0;
    uint32_t FlashStatus = 0;

    /* Get valid Page for read operation */
    ValidPage = Flash_EE_FindValidPage(GET_VALID_PAGE_READ);

    if (ValidPage == NO_VALID_PAGE)
    {
        return 1;
    }

    /* Calculate the valid Page end Address */
    PageEndAddress =
        FLASH_EE_START_ADDRESS + ValidPage * FLASH_EE_PAGE_SIZE + FLASH_EE_PAGE_SIZE - 4;

    /* Check if the page is full: G32F031 series Flash is erased, the data is 0xFFFFFFFF */
    if ((*(__IO uint32_t *)(PageEndAddress - 4)) != 0xFFFFFFFF)
    {
        /* If the page is full, transfer the data to new active page */
        if ((FlashStatus = Flash_EE_TransferToActivePage()) != 0)
        {
            return FlashStatus;
        }
    }

    return 0;
}

/*!
 * @brief       Transition state processing, one page state is valid, one page state is transfer,
 *              and the data is transferred to the transfer state page.
 *
 * @param       PageStatus0: Page0 status.
 *
 * @param       PageStatus1: Page1 status.
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
static uint32_t Flash_EE_ValidTransfer(uint32_t PageStatus0, uint32_t PageStatus1)
{
    uint32_t PageEraseAddress = 0;
    uint32_t FlashStatus = 0;

    /* Get the transfer page */
    if (PageStatus0 == PAGE_STATUS_TRANSFER)
    {
        PageEraseAddress = PAGE0_START_ADDRESS;
    }
    else
    {
        PageEraseAddress = PAGE1_START_ADDRESS;
    }

    /* Erase the transfer state page */
    if ((FlashStatus = Flash_EE_PageErase(PageEraseAddress)) != 0)
    {
        return FlashStatus;
    }

    /* Retransmit data */
    return Flash_EE_TransferToActivePage();
}

/*!
 * @brief       Transition state processing, a page state is erase, a page state is transfer,
 *              and the transfer state is changed to valid.
 *
 * @param       PageStatus0: Page0 status.
 *
 * @param       PageStatus1: Page1 status.
 *
 * @retval      Returns the fmc status. The value refer to uint32_t.
 *
 * @note
 */
static uint32_t Flash_EE_EraseTransfer(uint32_t PageStatus0, uint32_t PageStatus1)
{
    uint32_t PageStartAddress = 0;

    /* Get the transfer page */
    if (PageStatus0 == PAGE_STATUS_TRANSFER)
    {
        PageStartAddress = PAGE0_START_ADDRESS;
    }
    else
    {
        PageStartAddress = PAGE1_START_ADDRESS;
    }

    /* Mark the page status as valid */
    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
    *((uint32_t *)PageStartAddress) = PAGE_STATUS_VALID;
    while (DDL_FLASH_IsActiveFlag_BUSY())
        ;

    return 0;
}
