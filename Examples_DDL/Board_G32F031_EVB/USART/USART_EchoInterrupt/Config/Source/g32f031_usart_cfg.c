/**
 * @file        g32f031_uart_cfg.c
 *
 * @brief       This file provides configuration support for USART
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
#define USART_TEST USART
/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   USART configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DDL_USART_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    DDL_USART_InitTypeDef USART_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_USART);
    DDL_RCC_REG_Lock();

    /* Configure the USART TX pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure the USART RX pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    USART_InitStruct.BaudRate = 115200U;
    USART_InitStruct.DataWidth = DDL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = DDL_USART_STOPBITS_1;
    USART_InitStruct.Parity = DDL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = DDL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = DDL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = DDL_USART_OVERSAMPLING_16;
    DDL_USART_Init(USART_TEST, &USART_InitStruct);

    DDL_USART_EnableIT_RXNE(USART_TEST);
    NVIC_SetPriority(USART_IRQn, 0U);
    NVIC_EnableIRQ(USART_IRQn);

    DDL_USART_Enable(USART_TEST);
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
    DDL_USART_TransmitData8(USART_TEST, (uint8_t)ch);
    while (DDL_USART_IsActiveFlag_TXE(USART_TEST) == RESET)
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
    DDL_USART_TransmitData8(USART_TEST, (uint8_t)ch);
    while (DDL_USART_IsActiveFlag_TXE(USART_TEST) == RESET)
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
