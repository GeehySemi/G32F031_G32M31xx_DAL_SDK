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
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define IS_MASTER_BOARD

#define SLAVE_ADDRESS 0xA0U
#define WRITE_OPTION  0x00U
#define READ_OPTION   0x01U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
static char txBuffer[] = "I2C_TwoBoardsPolling\r\n";
static char rxBuffer[50U];
static volatile uint32_t TxCount = 0U;
static char *pTxData;
static volatile uint32_t RxCount = 0U;
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

#if defined(IS_MASTER_BOARD)
    pTxData = txBuffer;
    TxCount = strlen(txBuffer);

    /* Generate start condition */
    DDL_I2C_GenerateStartCondition(I2C);
    /* Make sure start condition generated and then clear start flag */
    while (DDL_I2C_IsActiveFlag_Start(I2C) == 0)
    {
    }
    /* Send address and option to slave */
    DDL_I2C_TransmitData8(I2C, (SLAVE_ADDRESS | WRITE_OPTION));

    /* Wait for address send finish */
    while (DDL_I2C_IsActiveFlag_ADDR(I2C) == 0)
    {
    }
    /* Clear address flag to start communication */
    DDL_I2C_ClearFlag_ADDR(I2C);

    while (TxCount)
    {
        /* Transmit data to slave */
        while (DDL_I2C_IsActiveFlag_TXE(I2C) == 0)
        {
        }
        DDL_I2C_TransmitData8(I2C, *pTxData);
        pTxData++;
        TxCount--;
    }

    while (DDL_I2C_IsActiveFlag_BTF(I2C) == 0)
    {
    }

    /* Generate stop condition */
    DDL_I2C_GenerateStopCondition(I2C);

    DDL_mDelay(5);

    pRxData = rxBuffer;
    RxCount = strlen(txBuffer);

    /* Generate start condition */
    DDL_I2C_GenerateStartCondition(I2C);
    /* Make sure start condition generated and then clear start flag */
    while (DDL_I2C_IsActiveFlag_Start(I2C) == 0)
    {
    }
    /* Send address and option to slave */
    DDL_I2C_TransmitData8(I2C, (SLAVE_ADDRESS | READ_OPTION));

    /* Wait for address send finish */
    while (DDL_I2C_IsActiveFlag_ADDR(I2C) == 0)
    {
    }
    /* Clear address flag to start communication */
    DDL_I2C_ClearFlag_ADDR(I2C);

    while (RxCount > 1U)
    {
        while (DDL_I2C_IsActiveFlag_RXNE(I2C) == 0)
        {
        }
        *pRxData = DDL_I2C_ReceiveData8(I2C);
        pRxData++;
        RxCount--;
    }

    /* NACK the last data */
    DDL_I2C_AcknowledgeNextData(I2C, DDL_I2C_NACK);
    while (DDL_I2C_IsActiveFlag_RXNE(I2C) == 0)
    {
    }
    *pRxData = DDL_I2C_ReceiveData8(I2C);

    /* Generate stop condition */
    DDL_I2C_GenerateStopCondition(I2C);

#else
    pRxData = rxBuffer;
    pTxData = rxBuffer;

    /* Wait for receive address */
    while (DDL_I2C_IsActiveFlag_ADDR(I2C) == 0)
    {
    }
    /* Clear address flag to start communication */
    DDL_I2C_ClearFlag_ADDR(I2C);

    /* Receive data from master until stop condition is detected */
    while (DDL_I2C_IsActiveFlag_STOP(I2C) == 0)
    {
        if (DDL_I2C_IsActiveFlag_RXNE(I2C))
        {
            *pRxData = DDL_I2C_ReceiveData8(I2C);
            pRxData++;
        }
    }

    DDL_I2C_ClearFlag_STOP(I2C);

    /* Wait for receive address */
    while (DDL_I2C_IsActiveFlag_ADDR(I2C) == 0)
    {
    }
    /* Clear address flag to start communication */
    DDL_I2C_ClearFlag_ADDR(I2C);

    /* Transmit data to master until stop condition is detected */
    while (DDL_I2C_IsActiveFlag_STOP(I2C) == 0)
    {
        if (DDL_I2C_IsActiveFlag_TXE(I2C))
        {
            DDL_I2C_TransmitData8(I2C, *pTxData);
            pTxData++;
        }
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
