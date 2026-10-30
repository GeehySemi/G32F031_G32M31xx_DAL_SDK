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

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
static void ATMR_UPDATE_IntRequest(void);

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
    }
}

/**
 * @brief  ATMR interrupt service routine.
 *
 * @param  None
 *
 * @retval None
 */
void ATMR_ISR(void)
{
    if (DDL_ATMR_IsActiveFlag_UPDATE(ATMR))
    {
        DDL_ATMR_ClearFlag_UPDATE(ATMR);
        ATMR_UPDATE_IntRequest();
    }
}

/* Private functions ******************************************************/

/**
 * @brief  ATMR UPDATE interrupt request function.
 *
 * @param  None
 *
 * @retval None
 */
static void ATMR_UPDATE_IntRequest(void)
{
    BOARD_LED_Toggle(LED2);
}
