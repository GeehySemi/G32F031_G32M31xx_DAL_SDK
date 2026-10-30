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
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
char txBufferStr[] = "UART_TwoBoardsDMA\r\n";
char rxBuffer[50U];
uint32_t txBuffer[sizeof(txBufferStr)/sizeof(txBufferStr[0])];
static volatile uint32_t uartRxFlag = 1U;

/* Private function prototypes ********************************************/
static void DMA_TC0_IntRequest(void);
static void DMA_TC1_IntRequest(void);
static uint8_t BufferCmp(uint8_t *buf1, uint8_t *buf2, uint32_t size);
static void CharToUint32(uint32_t *buffer, char *str, uint32_t size);
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

    /* Wait for USART reception complete */
    CharToUint32(txBuffer, txBufferStr, strlen(txBufferStr));

    DDL_DMA_EnableChannel(DMA, DDL_DMA_CHANNEL_1);
    DDL_DMA_EnableChannel(DMA, DDL_DMA_CHANNEL_0);

    while (uartRxFlag)
    {
    }

    if (!BufferCmp((uint8_t *)txBufferStr, (uint8_t *)rxBuffer, strlen(txBufferStr)))
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

/**
 * @brief  DMA Channel 0 interrupt service routine
 *
 * @param  None
 *
 * @retval None
 */
void DMA_CH0_ISR(void)
{
    if (DDL_DMA_IsActiveFlag_TC0(DMA))
    {
        DMA_TC0_IntRequest();
    }
}

/**
 * @brief  DMA Channel 1 interrupt service routine
 *
 * @param  None
 *
 * @retval None
 */
void DMA_CH1_ISR(void)
{
    if (DDL_DMA_IsActiveFlag_TC1(DMA))
    {
        DMA_TC1_IntRequest();
    }
}

/* Private functions ******************************************************/

/**
 * @brief  DMA TC0 interrupt request function
 *
 * @param  None
 *
 * @retval None
 */
static void DMA_TC0_IntRequest(void)
{
    DDL_DMA_ClearFlag_TC0(DMA);
    DDL_DMA_ClearFlag_HT0(DMA);
    uartRxFlag = 0U;
}

/**
 * @brief  DMA Channel 1 TC1 interrupt request function
 *
 * @param  None
 *
 * @retval None
 */
static void DMA_TC1_IntRequest(void)
{
    DDL_DMA_ClearFlag_TC1(DMA);
    DDL_DMA_ClearFlag_HT1(DMA);
}

/**
 * @brief   Buffer conversion from char to uint32_t
 *
 * @param   buffer: 32bit buffer to be converted
 *
 * @param   str: 8bit buffer to be converted
 *
 * @param   size: Buffer size
 *
 * @retval  None
 */
static void CharToUint32(uint32_t *buffer, char *str, uint32_t size)
{
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        buffer[i] = (uint32_t)str[i];
    }
}

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
