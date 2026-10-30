/**
 * @file        g32f031_tmr_cfg.c
 *
 * @brief       This file provides all configuration support for TMR
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
 * @brief   ATMR configuration
 *
 * @param   None
 *
 * @retval  None
 */
void ATMR_Config(void)
{
    DDL_ATMR_InitTypeDef ATMR_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_ATMR);
    DDL_RCC_REG_Lock();

    ATMR_InitStruct.Prescaler = 6399;
    ATMR_InitStruct.CounterMode = DDL_ATMR_COUNTERMODE_UP;
    ATMR_InitStruct.Autoreload = 4999;
    ATMR_InitStruct.ClockDivision = DDL_ATMR_CLOCKDIVISION_DIV1;
    ATMR_InitStruct.RepetitionCounter = 0;
    DDL_ATMR_Init(ATMR, &ATMR_InitStruct);

    DDL_ATMR_EnableIT_UPDATE(ATMR);

    NVIC_SetPriority(ATMR_BRK_UP_TRG_COM_IRQn, 3U);
    NVIC_EnableIRQ(ATMR_BRK_UP_TRG_COM_IRQn);

    DDL_ATMR_EnableCounter(ATMR);
}
