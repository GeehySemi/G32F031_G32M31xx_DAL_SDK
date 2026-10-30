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
 * @brief   BTMR configuration
 *
 * @param   None
 *
 * @retval  None
 */
void BTMR_Config(void)
{
    DDL_BTMR_InitTypeDef BTMR_InitStruct = {0};
    DDL_BTMR_OC_InitTypeDef BTMR_OC_InitStruct = {0};
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_BTMR0);
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOB);
    DDL_RCC_REG_Lock();

    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_HIGH;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_5;
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_0;
    DDL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    DDL_GPIO_REG_Lock(GPIOB);

    BTMR_InitStruct.Prescaler = 63;
    BTMR_InitStruct.CounterMode = DDL_BTMR_COUNTERMODE_UP;
    BTMR_InitStruct.Autoreload = 999;
    DDL_BTMR_Init(BTMR0, &BTMR_InitStruct);

    BTMR_OC_InitStruct.OCMode = DDL_BTMR_OCMODE_TOGGLE;
    BTMR_OC_InitStruct.OCState = DDL_BTMR_OCSTATE_ENABLE;
    BTMR_OC_InitStruct.CompareValue = 500;
    BTMR_OC_InitStruct.OCPolarity = DDL_BTMR_OCPOLARITY_HIGH;
    DDL_BTMR_OC_Init(BTMR0, DDL_BTMR_CHANNEL_CH0, &BTMR_OC_InitStruct);

    DDL_BTMR_EnableCounter(BTMR0);
}
