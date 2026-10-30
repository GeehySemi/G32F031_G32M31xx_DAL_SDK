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
#include <stdio.h>

/* Private includes *******************************************************/

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

    if (DDL_RCC_IsActiveFlag_WWDTRST())
    {
        printf("WWDT reset occurred\r\n");
        DDL_RCC_ClearFlag_WWDTRST();
    }

    printf("Start feeding dog\r\n");
    DDL_WWDT_Enable(WWDT);

    /* Infinite loop */
    while (1)
    {
        DDL_WWDT_SetCounter(WWDT, 0x7F);
        DDL_mDelay(30);
        BOARD_LED_Toggle(LED2);
    }
}

/**
 * @brief   EINT interrupt service routine
 *
 * @param   None
 *
 * @retval  None
 */
void EINT0_1_ISR(void)
{
    while (1)
    {
    }
}
