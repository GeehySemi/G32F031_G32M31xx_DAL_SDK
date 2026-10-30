/**
 * @file        g32f031_iwdt_cfg.c
 *
 * @brief       This file provides configuration support for IWDT
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
#include "g32f031_device_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   IWDT configuration
 *
 * @param   None
 *
 * @retval  None
 */
void IWDT_Config(void)
{
    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_IWDT);
    DDL_RCC_REG_Lock();

    /* Clock is 1000Hz (32K / 32) */
    DDL_IWDT_Enable(IWDT);
    DDL_IWDT_EnableWriteAccess(IWDT);
    DDL_IWDT_SetPrescaler(IWDT, DDL_IWDT_PSC_32);
    DDL_IWDT_SetReloadCounter(IWDT, 0xFFFU);
    DDL_IWDT_ReloadCounter(IWDT);
}
