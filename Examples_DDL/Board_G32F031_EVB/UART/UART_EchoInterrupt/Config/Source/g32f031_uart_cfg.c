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
    DDL_UART_Init(UART_TEST, &UART_InitStruct);

    DDL_UART_EnableIT_RXNE(UART_TEST);
    NVIC_SetPriority(UART_IRQn, 0U);
    NVIC_EnableIRQ(UART_IRQn);

    DDL_UART_Enable(UART_TEST);
}

#if defined(__CC_ARM) || defined(__ICCARM__) ||                                                    \
    (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
 * @brief       Redirect C Library function printf to serial port.
 *              After Redirection, you can use printf function.
 *
 * @param       ch:  The characters that need to be send.
 *
 * @param       *f:  pointer to a FILE that can recording all information
 *              needed to control a stream
 *
 * @retval      The characters that need to be send.
 *
 * @note
 */
int fputc(int ch, FILE *f)
{
    /* send a byte of data to the serial port */
    DDL_UART_TransmitData8(UART_TEST, (uint8_t)ch);
    while (DDL_UART_IsActiveFlag_TXE(UART_TEST) == RESET)
    {
    }

    return (ch);
}

#elif defined(__GNUC__)

/*!
 * @brief       Redirect C Library function printf to serial port.
 *              After Redirection, you can use printf function.
 *
 * @param       ch:  The characters that need to be send.
 *
 * @retval      The characters that need to be send.
 *
 * @note
 */
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    DDL_UART_TransmitData8(UART_TEST, (uint8_t)ch);
    while (DDL_UART_IsActiveFlag_TXE(UART_TEST) == RESET)
    {
    }

    return ch;
}

/*!
 * @brief       Redirect C Library function printf to serial port.
 *              After Redirection, you can use printf function.
 *
 * @param       file:  Meaningless in this function.
 *
 * @param       *ptr:  Buffer pointer for data to be sent.
 *
 * @param       len:  Length of data to be sent.
 *
 * @retval      The characters that need to be send.
 *
 * @note
 */
int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
