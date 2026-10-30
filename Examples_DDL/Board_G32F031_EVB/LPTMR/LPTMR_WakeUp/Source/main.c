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

    printf("MCU Enter Low Power Mode\r\n");
    DDL_LPTMR_Enable(LPTMR);

    __WFI();

    printf("MCU Wake Up from Low Power Mode\r\n");

    /* Infinite loop */
    while (1)
    {
    }
}

/**
 * @brief   LPTMR IRQ Handler
 *
 * @param   None
 *
 * @retval  None
 */
void LPTMR_ISR(void)
{
    if (DDL_LPTMR_IsActiveFlag_WakeUp(LPTMR))
    {
        DDL_LPTMR_DisableIT_WakeUp(LPTMR);
    }
}
