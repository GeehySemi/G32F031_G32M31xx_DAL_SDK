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
static void GTMR_CC0_IntRequest(void);

/* External variables *****************************************************/
uint32_t ICValue = 0;
uint32_t DutyCycle = 0;
uint32_t Frequency = 0;
volatile uint32_t IrqFlag = 0;
DDL_RCC_ClocksTypeDef RCC_Clocks = {0};

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
            printf("DutyCycle: %lu%%, Frequency: %lu Hz\r\n",
                   (unsigned long)DutyCycle, (unsigned long)Frequency);
            IrqFlag = 0;
        }
    }
}

/**
 * @brief  GTMR interrupt service routine.
 *
 * @param  None
 *
 * @retval None
 */
void GTMR_ISR(void)
{
    if (DDL_GTMR_IsActiveFlag_CC0(GTMR))
    {
        GTMR_CC0_IntRequest();
    }
}

/* Private functions ******************************************************/

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
    ICValue = DDL_GTMR_IC_GetCaptureCH0(GTMR);

    if (ICValue != 0)
    {
        DutyCycle = ((DDL_GTMR_IC_GetCaptureCH1(GTMR) + 1) * 100) / ICValue;
        DDL_RCC_GetSystemClocksFreq(&RCC_Clocks);
        Frequency = RCC_Clocks.PCLK_Frequency / ICValue;
        IrqFlag = 1;
    }
    else
    {
        DutyCycle = 0;
        Frequency = 0;
    }
}
