/**
 * @file        g32f031_uart_cfg.c
 *
 * @brief       This file provides configuration support for UART
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
#include <stdio.h>

/* Private macro **********************************************************/
#define UART_TEST UART
/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   UART configuration
 *
 * @param   None
 *
 * @retval  None
 */
void UART_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    DDL_UART_InitTypeDef UART_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOB);
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_UART);
    DDL_RCC_REG_Lock();

    /* Configure the UART TX pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure the UART RX pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOB);

    UART_InitStruct.BaudRate = 115200U;
    UART_InitStruct.DataWidth = DDL_UART_DATAWIDTH_8B;
    UART_InitStruct.StopBits = DDL_UART_STOPBITS_1;
    UART_InitStruct.Parity = DDL_UART_PARITY_NONE;
    UART_InitStruct.TransferDirection = DDL_UART_DIRECTION_TX_RX;
    DDL_UART_Init(UART, &UART_InitStruct);

    DDL_UART_Enable(UART);
}
