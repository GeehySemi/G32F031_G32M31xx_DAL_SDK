/**
 * @file        iap_flash.c
 *
 * @brief       Include IAP flash operation
 *
 * @version     V1.0.0
 *
 * @date        2026-01-15
 *
 * @attention
 *
 *  Copyright (C) 2026 Geehy Semiconductor
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
#include "iap_flash.h"

/* Private includes *******************************************************/
#include <stdio.h>

/* Private macro **********************************************************/
#define FLASH_SECTOR_SIZE 0x200U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint32_t JumpAddress;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/
pFunction Jump_To_Application;

/**
 * @brief   Unlocks Flash for write access
 *
 * @param   None
 *
 * @retval  None
 *
 */
void IAP_FLASH_Init(void)
{
    /* Unlock the flash memory */
    DDL_FLASH_REG_Unlock();
    DDL_FLASH_MAIN_Unlock();

    /* Clear all Flash flags */
    DDL_FLASH_ClearFlag_KEY();
    DDL_FLASH_ClearFlag_OPEND();
    DDL_FLASH_ClearFlag_RPT();
}

/**
 * @brief   Erase of all user flash area
 *
 * @param   application
 *          @arg IAP_APP1 : IAP_Application1
 *          @arg IAP_APP2 : IAP_Application2
 *
 * @retval  SUCCESS: user flash area successfully erased
 *          ERROR: error occurred
 */
uint32_t IAP_FLASH_Erase(IAP_APP_T application)
{
    uint32_t numberOfPage = 0x0;
    uint32_t flashDestination = 0x0;

    /* Configure the Flash to re-program the Flash */
    IAP_FLASH_Init();

    if (application == IAP_APP1)
    {
        numberOfPage = USER_APP1_FLASH_SIZE / FLASH_SECTOR_SIZE;
        flashDestination = USER_APP1_START_ADDRESS;
    }
    else
    {
        numberOfPage = USER_APP2_FLASH_SIZE / FLASH_SECTOR_SIZE;
        flashDestination = USER_APP2_START_ADDRESS;
    }

    /* Erase sector */
    while (numberOfPage--)
    {
        DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_SECTORERASE);
        *((uint32_t *)flashDestination) = 0xa5a5;
        while (DDL_FLASH_IsActiveFlag_BUSY())
            ;
        if (DDL_FLASH_IsActiveFlag_OPEND() == 0)
        {
            return (ERROR);
        }
        flashDestination += FLASH_SECTOR_SIZE;
    }

    return (SUCCESS);
}

/**
 * @brief   Write the appoint data buffer in flash
 *
 * @param   address: start address for writing data buffer
 *
 * @param   data: pointer on data buffer
 *
 * @param   length: length of data buffer (unit is 32-bit word)
 *
 * @param   application
 *          @arg IAP_APP1 : IAP_Application1
 *          @arg IAP_APP2 : IAP_Application2
 *
 * @retval  SUCCESS: data successfully written to Flash memory
 *          ERROR: Error occurred while writing data in Flash memory
 *
 * @note    buffer data are 32-bit aligned
 */
uint32_t IAP_FLASH_Write(uint32_t address, uint8_t *data, uint16_t length, IAP_APP_T application)
{
    uint32_t i;
    uint32_t endAddress;
    uint32_t dataWord;

    if (application == IAP_APP1)
    {
        endAddress = USER_APP1_END_ADDRESS - 3U;
    }
    else
    {
        endAddress = USER_APP2_END_ADDRESS - 3U;
    }

    for (i = 0U; i < length;)
    {
        if (address > endAddress)
        {
            return (ERROR);
        }

        /* The operation will be done by word and Check the written value */
        DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);

        dataWord = (*(data + i + 3) << 24) + (*(data + i + 2) << 16) + (*(data + i + 1) << 8) +
                   (*(data + i));
        *((uint32_t *)address) = dataWord;
        while (DDL_FLASH_IsActiveFlag_BUSY())
        {
        }

        if (*(uint32_t *)address != dataWord)
        {
            return (ERROR);
        }

        address += 4;
        i += 4;
    }

    return (SUCCESS);
}

/**
 * @brief   Read flash write protection status
 *
 * @param   None
 *
 * @retval  Flash write protection status
 */
uint16_t IAP_OPT_ReadWriteProtect(void)
{
    uint16_t ProtectedSECTOR = 0;

    DDL_FLASH_REG_Unlock();
    DDL_FLASH_MAIN_Unlock();

    /* Get flash read protect status */
    // ProtectedSECTOR = __DDL_FLASH_GET_RPT();

    DDL_FLASH_MAIN_Lock();
    DDL_FLASH_REG_Lock();

    return ProtectedSECTOR;
}

/**
 * @brief   Jump to user application
 *
 * @param   application : IAP_APP1 or IAP_APP2
 *
 * @retval  None
 */
void IAP_Jump2App(IAP_APP_T application)
{
    uint32_t address;

    /* Lock the Program memory */
    DDL_FLASH_MAIN_Lock();
    DDL_FLASH_REG_Lock();

    if (application == IAP_APP1)
    {
        address = USER_APP1_START_ADDRESS;
    }
    else
    {
        address = USER_APP2_START_ADDRESS;
    }

    /* Jump to user application */
    JumpAddress = *(__IO uint32_t *)(address + 4U);
    Jump_To_Application = (pFunction)JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)address);

    /* Jump to application */
    Jump_To_Application();
}
