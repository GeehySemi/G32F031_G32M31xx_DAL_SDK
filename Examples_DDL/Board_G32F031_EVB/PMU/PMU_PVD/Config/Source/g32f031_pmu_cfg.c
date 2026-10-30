/**
 * @file        g32f031_pmu_cfg.c
 *
 * @brief       This file provides configuration support for PMU
 *
 * @version     V1.0.1
 *
 * @date        2026-09-30
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
 * @brief   PMU configuration
 *
 * @param   None
 *
 * @retval  None
 */
void PMU_Config(void)
{
    DDL_EINT_InitTypeDef EINT_InitStruct = {0U};

    /* Configure the PVD Level and mode */
    DDL_PMU_REG_Unlock();
    DDL_PMU_SetPVDVoltageThreshold(DDL_PMU_PVD_THRESHOLD_8);

    /* Enable the PVD Interrupt */
    DDL_PMU_EnablePVDLT();
    DDL_PMU_EnablePVDHT();
    DDL_PMU_EnableIT_PVD();
    DDL_PMU_REG_Lock();

    /*--------------------- EINT Mode Configuration ------------------------*/
    DDL_RCC_REG_Unlock();
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_EINT);
    DDL_RCC_REG_Lock();

    EINT_InitStruct.Line_0_31 = DDL_EINT_LINE_16;
    EINT_InitStruct.LineCommand = ENABLE;
    EINT_InitStruct.Mode = DDL_EINT_MODE_IT;
    EINT_InitStruct.Trigger = DDL_EINT_TRIGGER_RISING_FALLING;
    DDL_EINT_Init(&EINT_InitStruct);

    /* Enable Interrupt */
    NVIC_SetPriority(PVD_IRQn, 0U);
    NVIC_EnableIRQ(PVD_IRQn);

    /* Enable the PVD Output */
    DDL_PMU_REG_Unlock();
    DDL_PMU_EnablePVD();
    DDL_PMU_REG_Lock();
}
