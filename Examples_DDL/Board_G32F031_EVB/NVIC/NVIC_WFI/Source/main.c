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
#include <stdio.h>
#include "g32f031_device_cfg.h"
#include "board_g32f031_evb_ddl.h"

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
__IO uint8_t lowPowerMode = 0U;

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
        if (lowPowerMode == 1U)
        {
            BOARD_LED_Off(LED2);

            /* Suspend system tick */
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

            printf("Enter sleep mode\r\n");

            /* Wait for interrupt */
            /* Clear SLEEPDEEP bit of Cortex System Control Register */
            CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
            /* Wait for interrupt */
            __SEV();
            __WFI();

            /* Resume system tick */
            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
            printf("Exit sleep mode\r\n");
            lowPowerMode = 0U;
        }
        else
        {
            BOARD_LED_Toggle(LED2);
            DDL_mDelay(500U);
            printf("Running...\r\n");
        }
    }
}

/**
 * @brief  EINT interrupt handler
 *
 * @param  None
 *
 * @retval None
 */
void EINT_ISR(void)
{
    if (DDL_EINT_IsActiveFlag_0_31(DDL_EINT_LINE_10))
    {
        DDL_EINT_ClearFlag_0_31(DDL_EINT_LINE_10);
        BOARD_LED_Toggle(LED2);
        lowPowerMode = 1U;
    }
}
