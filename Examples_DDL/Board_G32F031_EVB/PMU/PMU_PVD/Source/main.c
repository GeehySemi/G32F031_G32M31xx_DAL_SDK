/**
 * @file        main.c
 *
 * @brief       Main program body
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
#include "main.h"

/* Private includes *******************************************************/
#include "board_g32f031_evb_ddl.h"
#include <stdio.h>

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    /* Device configuration */
    Device_Config();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(500U);
    }
}

/**
 * @brief  EINT line detection callbacks
 *
 * @param  None
 *
 * @retval None
 */
void PVD_ISR(void)
{
    if (DDL_PMU_IsActiveFlag_PVDF())
    {
        printf("PVD detection level high than 4.8V\r\n");
    }
    else
    {
        printf("PVD detection level low than 4.7V\r\n");
    }

    DDL_PMU_REG_Unlock();
    DDL_PMU_DisablePVD();
    DDL_PMU_ClearFlag_PVDF();
    DDL_PMU_REG_Lock();

    DDL_EINT_ClearFlag_0_31(DDL_EINT_LINE_16);
}
