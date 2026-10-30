/**
 * @file        main.c
 *
 * @brief       Main program body
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
#include "main.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define FLASH_USER_ADDR ((uint32_t)0x00007000U)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint32_t sectorError = 0;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    /* Device configuration */
    Device_Config();

    /* Unlock the Flash */
    DDL_FLASH_REG_Unlock();
    DDL_FLASH_MAIN_Unlock();

    /* Read address data */
    printf("Before erase sector, data of 0x00007000 address = 0x%08lX\r\n",
           *(volatile uint32_t *)FLASH_USER_ADDR);

    /* Erase sector */
    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_SECTORERASE);
    *((uint32_t *)FLASH_USER_ADDR) = 0xA5A5;
    while (DDL_FLASH_IsActiveFlag_BUSY())
    {
    }

    /* Read address data */
    printf("After erase sector, data of 0x00007000 address = 0x%08lX\r\n",
           *(volatile uint32_t *)FLASH_USER_ADDR);

    DDL_FLASH_SetOperationMode(DDL_FLASH_OPERATE_WRITE);
    *((uint32_t *)FLASH_USER_ADDR) = 0x12345678U;
    while (DDL_FLASH_IsActiveFlag_BUSY())
    {
    }

    /* Read data again */
    printf("After program word, data of 0x00007000 address = 0x%08lX\r\n",
           *(volatile uint32_t *)FLASH_USER_ADDR);

    /* Lock the Flash */
    DDL_FLASH_MAIN_Lock();

    /* Infinite loop */
    while (1)
    {
    }
}
