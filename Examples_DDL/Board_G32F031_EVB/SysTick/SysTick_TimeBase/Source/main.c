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
static __IO uint32_t timingDelay;

/* Private function prototypes ********************************************/
static void SysTick_Init(void);
static void SysTick_Delay_ms(uint32_t nTime);

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

    SysTick_Init();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        SysTick_Delay_ms(500U);
    }
}

/**
 * @brief   Decrements the timingDelay
 *
 * @param   None
 *
 * @retval  None
 */
void TimingDelay_Decrement(void)
{
    if (timingDelay != 0)
    {
        timingDelay--;
    }
}

/**
 * @brief   Start sysTick
 *
 * @param   None
 *
 * @retval  None
 */
static void SysTick_Init(void)
{
    /* SystemFrequency / 1000 = 1ms */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* Capture error */
        while (1)
        {
        }
    }
}

/**
 * @brief   Precise delay
 *
 * @param   nTime in milliseconds
 *
 * @retval  None
 */
static void SysTick_Delay_ms(uint32_t nTime)
{
    timingDelay = nTime;
    while (timingDelay != 0)
    {
    }
}
