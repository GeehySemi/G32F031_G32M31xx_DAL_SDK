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
 * @brief   GTMR configuration
 *
 * @param   None
 *
 * @retval  None
 */
void GTMR_Config(void)
{
    DDL_GTMR_InitTypeDef GTMR_InitStruct = {0};
    DDL_GTMR_IC_InitTypeDef GTMR_IC_InitStruct = {0};
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_GTMR);
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_RCC_REG_Lock();

    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_HIGH;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_4;
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_7;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    DDL_GPIO_REG_Lock(GPIOA);

    GTMR_InitStruct.Prescaler = DDL_GTMR_ICPSC_DIV1;
    GTMR_InitStruct.CounterMode = DDL_GTMR_COUNTERMODE_UP;
    GTMR_InitStruct.Autoreload = 0xFFFFFFFF;
    GTMR_InitStruct.ClockDivision = DDL_GTMR_CLOCKDIVISION_DIV1;
    DDL_GTMR_Init(GTMR, &GTMR_InitStruct);

    GTMR_IC_InitStruct.ICPolarity = DDL_GTMR_IC_POLARITY_RISING;
    GTMR_IC_InitStruct.ICActiveInput = DDL_GTMR_ACTIVEINPUT_DIRECTTI;
    GTMR_IC_InitStruct.ICPrescaler = DDL_GTMR_ICPSC_DIV1;
    GTMR_IC_InitStruct.ICFilter = 0;
    DDL_GTMR_IC_Init(GTMR, DDL_GTMR_CHANNEL_CH0, &GTMR_IC_InitStruct);

    GTMR_IC_InitStruct.ICPolarity = DDL_GTMR_IC_POLARITY_FALLING;
    GTMR_IC_InitStruct.ICActiveInput = DDL_GTMR_ACTIVEINPUT_INDIRECTTI;
    DDL_GTMR_IC_Init(GTMR, DDL_GTMR_CHANNEL_CH1, &GTMR_IC_InitStruct);

    DDL_GTMR_SetTriggerInput(GTMR, DDL_GTMR_TS_TI0FP0);
    DDL_GTMR_SetSlaveMode(GTMR, DDL_GTMR_SLAVEMODE_RESET);
    DDL_GTMR_EnableMasterSlaveMode(GTMR);

    DDL_GTMR_EnableIT_CC0(GTMR);

    NVIC_SetPriority(GTMR_IRQn, 0U);
    NVIC_EnableIRQ(GTMR_IRQn);

    DDL_GTMR_EnableCounter(GTMR);
}
