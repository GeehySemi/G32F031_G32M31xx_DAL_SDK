/**
 * @file        board_g32f031_evb_ddl.c
 *
 * @brief       This file provides firmware functions to manage Leds and key buttons
 *
 * @version     V1.0.1
 *
 * @date        2026-09-30
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
#include "board_g32f031_evb_ddl.h"

#if defined(BOARD_G32F031_EVB)

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define LED_NUM    2U
#define BUTTON_NUM 2U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
GPIO_TypeDef *LED_PORT[LED_NUM] = {LED1_GPIO_PORT, LED2_GPIO_PORT};
const uint16_t LED_PIN[LED_NUM] = {LED1_PIN, LED2_PIN};

GPIO_TypeDef *BUTTON_PORT[BUTTON_NUM] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT};
const uint16_t BUTTON_PIN[BUTTON_NUM] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN};
const uint16_t BUTTON_EINT_LINE[BUTTON_NUM] = {KEY1_BUTTON_EINT_LINE, KEY2_BUTTON_EINT_LINE};
const uint16_t BUTTON_EINT_PIN_PORT[BUTTON_NUM] = {KEY1_BUTTON_EINT_PIN_PORT, KEY2_BUTTON_EINT_PIN_PORT};
const IRQn_Type BUTTON_IRQn[BUTTON_NUM] = {KEY1_BUTTON_EINT_IRQ_NUM, KEY2_BUTTON_EINT_IRQ_NUM};

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Board LED configuration
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *
 * @retval  BSP status
 */
uint32_t BOARD_LED_Config(BOARD_LED_T led)
{
    uint32_t status = 0U;

    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0U};

    /* Unlock RCC register */
    DDL_RCC_REG_Unlock();

    switch (led)
    {
        case LED1:
            LED1_GPIO_CLK_ENABLE();
            break;

        case LED2:
            LED2_GPIO_CLK_ENABLE();
            break;
        default:
            break;
    }

    /* Lock RCC register */
    DDL_RCC_REG_Lock();

    /* Configure the LED pin */
    GPIO_InitStruct.Pin = LED_PIN[led];
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    DDL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
    DDL_GPIO_SetOutputPin(LED_PORT[led], LED_PIN[led]);
    DDL_GPIO_REG_Lock(LED_PORT[led]);

    return status;
}

/**
 * @brief   Board button configuration
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *
 * @param       Button_Mode: Specifies Button mode.
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_MODE_GPIO: Button will be used as simple IO
 *              @arg BUTTON_MODE_EINT: Button will be connected to EINT line
 *                   with interrupt generation capability
 *
 * @retval  BSP status
 */
uint32_t BOARD_BUTTON_Config(BOARD_BUTTON_T button, BOARD_BUTTON_MODE_T mode)
{
    uint32_t status = 0U;

    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0U};
    DDL_EINT_InitTypeDef EINT_InitStruct = {0U};

    /* Unlock RCC register */
    DDL_RCC_REG_Unlock();

    switch (button)
    {
        case BUTTON_KEY1:
            KEY1_BUTTON_GPIO_CLK_ENABLE();
            KEY1_BUTTON_EINT_CLK_ENABLE();
            break;

        case BUTTON_KEY2:
            KEY2_BUTTON_GPIO_CLK_ENABLE();
            KEY2_BUTTON_EINT_CLK_ENABLE();
            break;

        default:
            break;
    }

    /* Lock RCC register */
    DDL_RCC_REG_Lock();

    if (mode == BUTTON_MODE_GPIO)
    {
        /* Configure the BUTTON pin */
        GPIO_InitStruct.Pin = BUTTON_PIN[button];
        GPIO_InitStruct.Mode = DDL_GPIO_MODE_INPUT;
        GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_ENABLE;
        GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
        GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
        DDL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
        DDL_GPIO_REG_Lock(BUTTON_PORT[button]);
    }

    if (mode == BUTTON_MODE_EINT)
    {
        /* Configure the BUTTON pin */
        GPIO_InitStruct.Pin = BUTTON_PIN[button];
        GPIO_InitStruct.Mode = DDL_GPIO_MODE_INPUT;
        GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_ENABLE;
        GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
        DDL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
        DDL_GPIO_REG_Lock(BUTTON_PORT[button]);

        /*--------------------- EINT Mode Configuration ------------------------*/
        EINT_InitStruct.Line_0_31 = BUTTON_EINT_LINE[button];
        EINT_InitStruct.LineCommand = ENABLE;
        EINT_InitStruct.Mode = DDL_EINT_MODE_IT;
        EINT_InitStruct.Trigger = DDL_EINT_TRIGGER_FALLING;

        DDL_EINT_SetEINTSource(BUTTON_EINT_PIN_PORT[button], BUTTON_EINT_LINE[button]);
        DDL_EINT_Init(&EINT_InitStruct);

        /* Enable and set Button EINT Interrupt to the lowest priority */
        NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[button]), 0x03U);
        NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));
    }

    return status;
}

/**
 * @brief   Turn on the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *
 * @retval  None
 */
void BOARD_LED_On(BOARD_LED_T led)
{
    DDL_GPIO_REG_Unlock(LED_PORT[led]);
    DDL_GPIO_ResetOutputPin(LED_PORT[led], LED_PIN[led]);
    DDL_GPIO_REG_Lock(LED_PORT[led]);
}

/**
 * @brief   Turn off the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *
 * @retval  None
 */
void BOARD_LED_Off(BOARD_LED_T led)
{
    DDL_GPIO_REG_Unlock(LED_PORT[led]);
    DDL_GPIO_SetOutputPin(LED_PORT[led], LED_PIN[led]);
    DDL_GPIO_REG_Lock(LED_PORT[led]);
}

/**
 * @brief   Toggles the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *
 * @retval  None
 */
void BOARD_LED_Toggle(BOARD_LED_T led)
{
    DDL_GPIO_REG_Unlock(LED_PORT[led]);
    DDL_GPIO_TogglePin(LED_PORT[led], LED_PIN[led]);
    DDL_GPIO_REG_Lock(LED_PORT[led]);
}

/**
 * @brief   Returns the selected button state
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *
 * @retval  The button GPIO pin value.
 */
uint32_t BOARD_BUTTON_GetState(BOARD_BUTTON_T button)
{
    uint32_t bitstatus;

    if ((DDL_GPIO_ReadInputPort(BUTTON_PORT[button]) & BUTTON_PIN[button]) != 0U)
    {
        bitstatus = 1U;
    }
    else
    {
        bitstatus = 0U;
    }
    return bitstatus;
}

#endif /* BOARD_G32F031_EVB */
