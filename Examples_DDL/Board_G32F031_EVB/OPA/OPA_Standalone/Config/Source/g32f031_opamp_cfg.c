/**
 * @file        g32f031_opamp_cfg.c
 *
 * @brief       This file provides configuration support for OPA
 *
 * @version     V1.0.0
 *
 * @date        2026-01-15
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
#include "g32f031_device_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   OPA configuration
 *
 * @param   None
 *
 * @retval  None
 */
void OPA0_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    DDL_OPA_InitTypeDef OPA_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_OPA);
    DDL_RCC_REG_Lock();

    /* Configure the OPA pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_0 | DDL_GPIO_PIN_1 | DDL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_DISABLE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    OPA_InitStruct.Channel = DDL_OPA_CHANNEL_0;
    OPA_InitStruct.GainSelect = DDL_OPA_INTERNALGAIN_10;
    OPA_InitStruct.InputControl = DDL_OPA_INCTRL_ENABLE;
    OPA_InitStruct.OutputControl = DDL_OPA_OUTCTRL_ENABLE;
    OPA_InitStruct.VCMSelect = DDL_OPA_VCMSEL_AVDD_0_5;
    DDL_OPA_Init(OPA, &OPA_InitStruct);
}
