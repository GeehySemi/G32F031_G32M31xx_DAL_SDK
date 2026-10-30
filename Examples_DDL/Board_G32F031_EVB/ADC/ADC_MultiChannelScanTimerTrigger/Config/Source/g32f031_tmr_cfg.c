/**
 * @file        g32f031_tmr_cfg.c
 *
 * @brief       This file provides configuration support for TMR
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
#include <stdio.h>

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   ADC configuration
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0U};
    DDL_ATMR_InitTypeDef ATMR_InitStruct;
    DDL_ATMR_OC_InitTypeDef ATMR_OC_InitStruct;

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_ATMR);
    DDL_RCC_REG_Lock();

    /* TMR channel pin configuration */
    DDL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_HIGH;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_3;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    DDL_ATMR_StructInit(&ATMR_InitStruct);
    ATMR_InitStruct.Prescaler = 63999U;
    ATMR_InitStruct.CounterMode = DDL_ATMR_COUNTERMODE_UP;
    ATMR_InitStruct.Autoreload = 999U;
    ATMR_InitStruct.ClockDivision = DDL_ATMR_CLOCKDIVISION_DIV1;
    ATMR_InitStruct.RepetitionCounter = 0U;
    DDL_ATMR_DisableARRPreload(ATMR);
    DDL_ATMR_Init(ATMR, &ATMR_InitStruct);
    if (DDL_ATMR_IsActiveFlag_UPDATE(ATMR))
    {
        DDL_ATMR_ClearFlag_UPDATE(ATMR);
    }

    /* Configure output channel */
    DDL_ATMR_OC_StructInit(&ATMR_OC_InitStruct);
    ATMR_OC_InitStruct.OCMode = DDL_ATMR_OCMODE_PWM1;
    ATMR_OC_InitStruct.OCState = ENABLE;
    ATMR_OC_InitStruct.OCNState = ENABLE;
    ATMR_OC_InitStruct.OCPolarity = DDL_ATMR_OCPOLARITY_HIGH;
    ATMR_OC_InitStruct.OCNPolarity = DDL_ATMR_OCPOLARITY_HIGH;
    ATMR_OC_InitStruct.OCIdleState = DDL_ATMR_OCIDLESTATE_HIGH;
    ATMR_OC_InitStruct.OCNIdleState = DDL_ATMR_OCIDLESTATE_HIGH;
    ATMR_OC_InitStruct.CompareValue = 500U;
    DDL_ATMR_OC_Init(ATMR, DDL_ATMR_CHANNEL_CH0, &ATMR_OC_InitStruct);

    DDL_ATMR_EnableAllOutputs(ATMR);

    DDL_ATMR_SetTriggerOutputMode0(ATMR, DDL_ATMR_TRGO_CC0IF);
    DDL_ATMR_DisableMasterSlaveMode(ATMR);
}
