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

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define BuffSize 56

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
volatile uint8_t TxIdx = 0;
volatile uint8_t RxIdx = 0;
uint8_t SPI_Buffer_TX[BuffSize] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
};
uint8_t SPI_Buffer_RX[BuffSize] = {0x00};

/* Private function prototypes ********************************************/
static void SPI_OVR_IntRequest(void);
static uint8_t BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size);

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

#ifdef SPI_MASTER
    printf("I am Master\r\n");

    DDL_SPI_Enable(SPI);
    while (RxIdx < BuffSize)
    {
        /* Receive Data */
        while (DDL_SPI_IsActiveFlag_RXNE(SPI) == RESET)
        {
        }
        SPI_Buffer_RX[RxIdx++] = DDL_SPI_ReceiveData8(SPI);
    }

    DDL_SPI_SetTransferDirection(SPI, DDL_SPI_HALF_DUPLEX_TX);

    while (TxIdx < BuffSize)
    {
        /* Send Data */
        while (DDL_SPI_IsActiveFlag_TXE(SPI) == RESET)
        {
        }
        DDL_SPI_TransmitData8(SPI, SPI_Buffer_TX[TxIdx++]);
    }

    while (DDL_SPI_IsActiveFlag_TXE(SPI) == RESET)
    {
    }
    while (DDL_SPI_IsActiveFlag_BUSY(SPI) == SET)
    {
    }
    DDL_SPI_Disable(SPI);
#else
    printf("I am Slave\r\n");

    DDL_SPI_Enable(SPI);
    while (TxIdx < BuffSize)
    {
        /* Send Data */
        while (DDL_SPI_IsActiveFlag_TXE(SPI) == RESET)
        {
        }
        DDL_SPI_TransmitData8(SPI, SPI_Buffer_TX[TxIdx++]);

    }

    while (DDL_SPI_IsActiveFlag_TXE(SPI) == RESET)
    {
    }
    while (DDL_SPI_IsActiveFlag_BUSY(SPI) == SET)
    {
    }
    DDL_SPI_SetTransferDirection(SPI, DDL_SPI_HALF_DUPLEX_RX);

    while (RxIdx < BuffSize)
    {
        /* Receive Data */
        while (DDL_SPI_IsActiveFlag_RXNE(SPI) == RESET)
        {
        }
        SPI_Buffer_RX[RxIdx++] = DDL_SPI_ReceiveData8(SPI);
    }
    DDL_SPI_Disable(SPI);
#endif

    /* Compare Buffer */
    if (BufferCompare(SPI_Buffer_TX, SPI_Buffer_RX, BuffSize))
    {
        BOARD_LED_On(LED2);
    }

    /* Infinite loop */
    while (1)
    {
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return 1 if buf1 = buf2. If not then return 0
 */
static uint8_t BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief   SPI interrupt service routine
 *
 * @param   None
 *
 * @retval  None
 */
void SPI_ISR(void)
{
    if (DDL_SPI_IsActiveFlag_OVR(SPI))
    {
        SPI_OVR_IntRequest();
    }
}

/* Private functions ******************************************************/

/**
 * @brief   SPI OVR interrupt request function
 *
 * @param   None
 *
 * @retval  None
 */
static void SPI_OVR_IntRequest(void)
{
    DDL_SPI_ClearFlag_OVR(SPI);
    DDL_SPI_Disable(SPI);
    printf("RX data overrun.\r\n");
}
