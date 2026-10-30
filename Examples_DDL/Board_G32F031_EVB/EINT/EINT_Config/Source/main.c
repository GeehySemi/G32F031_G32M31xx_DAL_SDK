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
static void EINT_0_1_IntRequest(void);

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
        DDL_mDelay(500);
        BOARD_LED_Toggle(LED2);
    }
}

/**
 * @brief   EINT interrupt server routine
 *
 * @param   None
 *
 * @retval  None
 */
void EINT_ISR(void)
{
    if (DDL_EINT_IsActiveFlag_0_31(DDL_EINT_LINE_1))
    {
        EINT_0_1_IntRequest();
    }
}

/**
 * @brief   EINT interrupt request handler
 *
 * @param   None
 *
 * @retval  None
 */
static void EINT_0_1_IntRequest(void)
{
    DDL_EINT_ClearFlag_0_31(DDL_EINT_LINE_1);
    printf("EINT 1 trigger\r\n");
}
