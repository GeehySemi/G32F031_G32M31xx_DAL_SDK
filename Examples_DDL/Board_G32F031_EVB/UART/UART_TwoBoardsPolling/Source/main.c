/**
 * @file        main.c
 *
 * @brief       Main program body
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
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Define firmware to switch between 1st board or 2nd board */
#define IS_FIRST_BOARD

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
static char txBuffer[] = "UART_TwoBoardsPolling\r\n";
static char rxBuffer[50U];
static volatile uint32_t uartTxCount = 0U;
static char *pTxData;
static volatile uint32_t uartRxCount = 0U;
static char *pRxData;

/* Private function prototypes ********************************************/
static uint8_t BufferCmp(uint8_t *buf1, uint8_t *buf2, uint32_t size);
static void Error_Handler(void);
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

#if defined(IS_FIRST_BOARD)
    pTxData = txBuffer;
    uartTxCount = strlen(txBuffer);
    while (uartTxCount)
    {
        while (DDL_UART_IsActiveFlag_TXE(UART) == 0)
        {
        }
        DDL_UART_TransmitData8(UART, *pTxData);
        pTxData++;
        uartTxCount--;
    }

    pRxData = rxBuffer;
    uartRxCount = strlen(txBuffer);
    while (uartRxCount)
    {
        while (DDL_UART_IsActiveFlag_RXNE(UART) == 0)
        {
        }
        *pRxData = DDL_UART_ReceiveData8(UART);
        pRxData++;
        uartRxCount--;
    }

#else
    pRxData = rxBuffer;
    uartRxCount = strlen(txBuffer);
    while (uartRxCount)
    {
        while (DDL_UART_IsActiveFlag_RXNE(UART) == 0)
        {
        }
        *pRxData = DDL_UART_ReceiveData8(UART);
        pRxData++;
        uartRxCount--;
    }

    pTxData = txBuffer;
    uartTxCount = strlen(txBuffer);
    while (uartTxCount)
    {
        while (DDL_UART_IsActiveFlag_TXE(UART) == 0)
        {
        }
        DDL_UART_TransmitData8(UART, *pTxData);
        pTxData++;
        uartTxCount--;
    }
#endif

    if (!BufferCmp((uint8_t *)txBuffer, (uint8_t *)rxBuffer, strlen(txBuffer)))
    {
        BOARD_LED_On(LED2);
        Error_Handler();
    }

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(500U);
    }
}

/* Private functions ******************************************************/

/**
 * @brief   Buffer compare
 *
 * @param   buf1: First buffer to be compared
 *
 * @param   buf2: Second buffer to be compared
 *
 * @param   size: Buffer size
 *
 * @retval  Comparison result(1 or 0)
 */
static uint8_t BufferCmp(uint8_t *buf1, uint8_t *buf2, uint32_t size)
{
    uint32_t i;

    uint8_t status = 1U;

    for (i = 0U; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            status = 0U;
            break;
        }
    }

    return status;
}

/**
 * @brief     Error handler
 *
 * @param     None
 *
 * @retval    None
 */
static void Error_Handler(void)
{
    /* When the function is needed, this function
       could be implemented in the user file
    */
    __disable_irq();
    while (1)
    {
    }
}
