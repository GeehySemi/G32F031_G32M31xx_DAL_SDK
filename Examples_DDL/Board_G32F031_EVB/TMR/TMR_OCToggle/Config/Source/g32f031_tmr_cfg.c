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
    DDL_ATMR_OC_InitTypeDef ATMR_OC_InitStruct = {0};
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_ATMR);
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_RCC_REG_Lock();

    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_HIGH;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_3;
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_10;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DDL_GPIO_PIN_11;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DDL_GPIO_PIN_12;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    DDL_GPIO_SetAF3Pin_10_15(GPIOA, DDL_GPIO_PIN_10, DDL_GPIO_AF3_ATMR_CH0);
    DDL_GPIO_SetAF3Pin_10_15(GPIOA, DDL_GPIO_PIN_11, DDL_GPIO_AF3_ATMR_CH1);
    DDL_GPIO_SetAF3Pin_10_15(GPIOA, DDL_GPIO_PIN_12, DDL_GPIO_AF3_ATMR_CH2);

    DDL_GPIO_REG_Lock(GPIOA);

    ATMR_InitStruct.Prescaler = 63;
    ATMR_InitStruct.CounterMode = DDL_ATMR_COUNTERMODE_UP;
    ATMR_InitStruct.Autoreload = 999;
    ATMR_InitStruct.ClockDivision = DDL_ATMR_CLOCKDIVISION_DIV1;
    ATMR_InitStruct.RepetitionCounter = 0;
    DDL_ATMR_Init(ATMR, &ATMR_InitStruct);

    ATMR_OC_InitStruct.OCMode = DDL_ATMR_OCMODE_TOGGLE;
    ATMR_OC_InitStruct.OCState = DDL_ATMR_OCSTATE_ENABLE;
    ATMR_OC_InitStruct.OCPolarity = DDL_ATMR_OCPOLARITY_HIGH;
    ATMR_OC_InitStruct.CompareValue = 400;
    DDL_ATMR_OC_Init(ATMR, DDL_ATMR_CHANNEL_CH0, &ATMR_OC_InitStruct);

    ATMR_OC_InitStruct.CompareValue = 600;
    DDL_ATMR_OC_Init(ATMR, DDL_ATMR_CHANNEL_CH1, &ATMR_OC_InitStruct);

    ATMR_OC_InitStruct.CompareValue = 800;
    DDL_ATMR_OC_Init(ATMR, DDL_ATMR_CHANNEL_CH2, &ATMR_OC_InitStruct);

    DDL_ATMR_EnableAllOutputs(ATMR);
    DDL_ATMR_EnableCounter(ATMR);
}
