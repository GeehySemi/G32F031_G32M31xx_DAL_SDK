/**
 * @file        g32f031_comp_cfg.c
 *
 * @brief       This file provides all configuration support for COMP
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
#include "g32f031_comp_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   COMP configuration
 *
 * @param   None
 *
 * @retval  None
 */
void COMP_Config(void)
{
    DDL_COMP0_InitTypeDef COMP0_InitStruct = {0};
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_COMP0);
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_RCC_REG_Lock();

    // COMP0 Output Pin
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_DISABLE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_7;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // COMP0 Input Plus Pin
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_ENABLE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // COMP0 Input Minus Pin
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_ENABLE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    COMP0_InitStruct.InputPlus = DDL_COMP0_INPUT_PLUS_PA7;
    COMP0_InitStruct.InputMinus = DDL_COMP0_INPUT_MINUS_PA8;
    COMP0_InitStruct.OutputPol = DDL_COMP0_OUTPUTPOL_NONINVERTED;
    COMP0_InitStruct.FilterPSC = DDL_COMP0_FILTERPSC_1;
    COMP0_InitStruct.FilterCFG = DDL_COMP0_FILTERCFG_1;
    COMP0_InitStruct.HsyP = DDL_COMP0_HYSP_DISABLE;
    COMP0_InitStruct.HsyN = DDL_COMP0_HYSN_DISABLE;
    DDL_COMP0_Init(COMP0, &COMP0_InitStruct);

    DDL_COMP0_Enable(COMP0);
}
