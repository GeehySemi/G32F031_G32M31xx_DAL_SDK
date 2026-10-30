/**
 * @file        g32f031_device_cfg.c
 *
 * @brief       This file provides all configuration support for device
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
#include "g32f031_device_cfg.h"

/* Private includes *******************************************************/
#include "board_g32f031_evb_ddl.h"
#include "g32f031_usart_cfg.h"
#include "g32f031_pmu_cfg.h"

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
static void RCM_PeripheralClkConfig(void);
static void NVIC_Config(void);
static void GPIO_Config(void);

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Device configuration
 *
 * @param   None
 *
 * @retval  None
 */
void Device_Config(void)
{
    /* Configure system clock */
    SysClk_Config();

    /* Configure peripheral clock */
    RCM_PeripheralClkConfig();

    /* Configure Board */
    BOARD_LED_Config(LED2);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);

    /* Configure GPIO */
    GPIO_Config();

    /* Configure NVIC */
    NVIC_Config();

    /* Configure peripherals */
    USART_Config();
    PMU_Config();
}

/**
 * @brief   Device reset
 *
 * @param   None
 *
 * @retval  None
 */
void Device_Reset(void)
{
    /* Reset Peripheral */

    /* Reset service */
}

/**
 * @brief   System clock configuration
 *
 * @param   None
 *
 * @retval  None
 */
void SysClk_Config(void)
{
    DDL_RCC_REG_Unlock();

    /* Set HSIEN */
    DDL_RCC_HSI_Enable();
    /* Wait for HSI READY */
    while (DDL_RCC_HSI_IsReady() != 1U)
    {
    }

    /* Configure FLASH latency */
    DDL_FLASH_SetLatency(DDL_FLASH_LATENCY3);

    /* Set HSI clock as system source clock, SYS_CLK = 64MHz */
    DDL_RCC_SetSysClkSource(DDL_RCC_SYS_CLKSOURCE_HSI);
    while (DDL_RCC_GetSysClkSource() != DDL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {
    }

    /* Configure HSI Prescaler, SYS_CLK = HSI_CLK/1 */
    DDL_RCC_SetHSIPrescaler(DDL_RCC_HSI_DIV_1);

    /* Configure AHB Prescaler, AHB_CLK = SYS_CLK/1 */
    DDL_RCC_SetAHBPrescaler(DDL_RCC_AHB_DIV_1);

    /* Configure APB prescaler, APB_CLK = AHB_CLK/1 */
    DDL_RCC_SetAPBPrescaler(DDL_RCC_APB_DIV_1);

    /* Update SystemCoreClock variable */
    SystemCoreClockUpdate();

    /* Set Systick to 1ms in using frequency set to SystemCoreClock */
    DDL_Init1msTick(SystemCoreClock);

    DDL_RCC_REG_Lock();
}

/**
 * @brief   Peripheral Clock configuration
 *
 * @param   None
 *
 * @retval  None
 */
static void RCM_PeripheralClkConfig(void)
{
}

/**
 * @brief   NVIC configuration
 *
 * @param   None
 *
 * @retval  None
 */
static void NVIC_Config(void)
{
    /* Set interrupt group priority */
    NVIC_SetPriorityGrouping(0x00000003U);
}

/**
 * @brief   GPIO configuration
 *
 * @param   None
 *
 * @retval  None
 */
static void GPIO_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0U};
    DDL_EINT_InitTypeDef EINT_InitStruct = {0U};

    /* Configure the EINT1 pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    DDL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOB);

    /*--------------------- EINT Mode Configuration ------------------------*/
    EINT_InitStruct.Line_0_31 = DDL_EINT_LINE_1;
    EINT_InitStruct.LineCommand = ENABLE;
    EINT_InitStruct.Mode = DDL_EINT_MODE_EVENT;
    EINT_InitStruct.Trigger = DDL_EINT_TRIGGER_FALLING;

    DDL_EINT_SetEINTSource(DDL_EINT_SOURCE_PORTB, DDL_EINT_LINE_1);
    DDL_EINT_Init(&EINT_InitStruct);
}

#if defined(USE_FULL_ASSERT)
/**
 * @brief   Assert failed handler
 *
 * @param   file :Pointer to the source file name
 *
 * @param   line :Error line source number
 *
 * @retval  None
 */
void AssertFailedHandler(uint8_t *file, uint32_t line)
{
    /* When the function is needed, this function
       could be implemented in the user file
    */
    (void)(file);
    (void)(line);
    while (1)
    {
    }
}
#endif /* USE_FULL_ASSERT */
