/**
 * @file        g32f031_lptmr_cfg.c
 *
 * @brief       This file provides configuration support for LPTMR
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
 * @brief   LPTMR configuration
 *
 * @param   None
 *
 * @retval  None
 */
void LPTMR_Config(void)
{
    DDL_LPTMR_InitTypeDef LPTMR_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_RCC_LSI_Enable();
    while (DDL_RCC_LSI_IsReady() != 1U)
    {
    }
    DDL_RCC_EnableAONPeripheralClock(DDL_RCC_AON_ENABLE_LPTMR);
    DDL_RCC_REG_Lock();

    LPTMR_InitStruct.Prescaler = 0xF;
    LPTMR_InitStruct.WakeUpValue = 5120;
    DDL_LPTMR_Init(LPTMR, &LPTMR_InitStruct);

    DDL_LPTMR_EnableIT_WakeUp(LPTMR);
    NVIC_SetPriority(LPTMR_IRQn, 0U);
    NVIC_EnableIRQ(LPTMR_IRQn);
}
