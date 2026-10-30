/**
 * @file        g32f031_wwdt_cfg.c
 *
 * @brief       This file provides configuration support for WWDT
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
 * @brief   WWDT configuration
 *
 * @param   None
 *
 * @retval  None
 */
void WWDT_Config(void)
{
    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_WWDT);
    DDL_RCC_REG_Lock();

    /* WWDT clock frequency = 64M / 4096 / 8 ≈ 2K Hz */
    DDL_WWDT_SetPrescaler(WWDT, DDL_WWDT_PRESCALER_8);
    DDL_WWDT_SetWindow(WWDT, 0x7F);
}
