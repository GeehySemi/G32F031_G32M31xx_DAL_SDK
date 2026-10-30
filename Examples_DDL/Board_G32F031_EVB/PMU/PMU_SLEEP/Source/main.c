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
static void PMU_SleepMode(void);

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

    PMU_SleepMode();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(500U);
    }
}

/**
 * @brief   PMU sleep mode
 *
 * @param   None
 *
 * @retval  None
 */
static void PMU_SleepMode(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    printf("Enter sleep mode\r\n");

    /* Unlock RCC register */
    DDL_RCC_REG_Unlock();

    /* Configure all GPIO as analog to reduce current consumption */
    /* Enable GPIOs clock */
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOB);

    GPIO_InitStruct.Pin = DDL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    GPIO_InitStruct.Pin = (DDL_GPIO_PIN_ALL & ~(DDL_GPIO_PIN_2 | DDL_GPIO_PIN_3 | DDL_GPIO_PIN_4));
    DDL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOB);

    /* Disable GPIOs clock */
    DDL_AHB_GRP1_DisableClock(DDL_AHB_GRP1_PERIPH_GPIOA);

    /* Lock RCC register */
    DDL_RCC_REG_Lock();

    /* Reset UART state for configure again and deinit UART */
    DDL_USART_DeInit(USART);

    /* Suspend system tick */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

    /* Wait for interrupt */
    /* Clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    /* Wait for interrupt */
    __WFI();

    /* Resume system tick */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    USART_Config();
    BOARD_LED_Config(LED2);
    printf("Exit sleep mode\r\n");
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
        wakeupFlag = 1U;
    }
}
