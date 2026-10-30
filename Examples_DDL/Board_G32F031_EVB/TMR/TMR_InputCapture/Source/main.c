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
uint32_t ICValue[2] = {0};
uint32_t State = 0;
uint32_t Frequency = 0;
volatile uint32_t UpdateTime = 0;
volatile uint32_t IrqFlag = 0;
DDL_RCC_ClocksTypeDef RCC_Clocks = {0};

/* Private function prototypes ********************************************/
static void GTMR_UPDATE_IntRequest(void);
static void GTMR_CC0_IntRequest(void);

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
        if (IrqFlag > 0)
        {
            printf("Frequency: %lu Hz\r\n", (unsigned long)Frequency);
            IrqFlag = 0;
        }
    }
}

/**
 * @brief  GTMR interrupt service function.
 *
 * @param  None
 *
 * @retval None
 */
void GTMR_ISR(void)
{
    if (DDL_GTMR_IsActiveFlag_UPDATE(GTMR))
    {
        GTMR_UPDATE_IntRequest();
    }

    if (DDL_GTMR_IsActiveFlag_CC0(GTMR))
    {
        GTMR_CC0_IntRequest();
    }
}

/* Private functions *******************************************************/

/**
 * @brief  GTMR UPDATE interrupt request function.
 *
 * @param  None
 *
 * @retval None
 */
static void GTMR_UPDATE_IntRequest(void)
{
    DDL_GTMR_ClearFlag_UPDATE(GTMR);
    UpdateTime++;
}

/**
 * @brief  GTMR CC0 interrupt request function.
 *
 * @param  None
 *
 * @retval None
 */
static void GTMR_CC0_IntRequest(void)
{
    DDL_GTMR_ClearFlag_CC0(GTMR);
    ICValue[State] = DDL_GTMR_IC_GetCaptureCH0(GTMR);

    if (State != 0)
    {
        DDL_RCC_GetSystemClocksFreq(&RCC_Clocks);
        Frequency =
            RCC_Clocks.PCLK_Frequency / (ICValue[1] + (UpdateTime * 0xFFFFFFFF) - ICValue[0]);
        IrqFlag = 1;
        State = 0;
    }
    else
    {
        Frequency = 0;
        UpdateTime = 0;
        State = 1;
    }
}
