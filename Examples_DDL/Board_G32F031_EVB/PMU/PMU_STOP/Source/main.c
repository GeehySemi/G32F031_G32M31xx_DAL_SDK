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
#include "g32f031_usart_cfg.h"
#include <stdio.h>

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
__IO uint32_t wakeupFlag = 0;

/* Private function prototypes ********************************************/
static void PMU_StopMode(void);

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

    /* Wait until USER button is pressed to enter the Low Power mode */
    while (BOARD_BUTTON_GetState(BUTTON_KEY1) == 0U)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(100);
    }

    /* Loop while USER Button is maintained pressed */
    while (BOARD_BUTTON_GetState(BUTTON_KEY1) != 0U)
    {
    }

    PMU_StopMode();
    if (wakeupFlag != 0)
    {
    }

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(500U);
    }
}

/**
 * @brief   PMU stop mode
 *
 * @param   None
 *
 * @retval  None
 */
static void PMU_StopMode(void)
{
    printf("Enter stop mode\r\n");

    /* Enter stop */
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    __WFI();

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    USART_Config();
    BOARD_LED_Config(LED2);
    DDL_mDelay(10U);
    printf("Exit stop mode\r\n");
}

/**
 * @brief  EINT line detection callbacks
 *
 * @param  None
 *
 * @retval None
 */
void EINT_ISR(void)
{
    /* EINT line interrupt detected */
    if (DDL_EINT_IsActiveFlag_0_31(DDL_EINT_LINE_2))
    {
        /* Clears the EINT line pending bits. */
        DDL_EINT_ClearFlag_0_31(DDL_EINT_LINE_2);
    }
}
