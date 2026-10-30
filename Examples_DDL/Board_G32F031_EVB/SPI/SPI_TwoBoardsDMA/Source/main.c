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

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define BuffSize 56

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint32_t txBuffer[BuffSize] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
};
uint32_t rxBuffer[BuffSize] = {0x00};
volatile uint8_t tcFlag = 0;

/* Private function prototypes ********************************************/
static void SPI_OVR_IntRequest(void);
static void DMA_CH0_TC_IntRequest(void);
static void DMA_CH1_TC_IntRequest(void);
static uint8_t BufferCompare(uint32_t *buf1, uint32_t *buf2, uint8_t size);

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
#else
    printf("I am Slave\r\n");
#endif

    DDL_SPI_Enable(SPI);

    /* Wait for SPI transmition finish */
    while ((tcFlag == 0) || DDL_SPI_IsActiveFlag_BUSY(SPI))
    {
    }

    DDL_SPI_Disable(SPI);

    /* Compare Buffer */
    if (BufferCompare(txBuffer, rxBuffer, BuffSize))
    {
        BOARD_LED_On(LED2);
        printf("SPI communication succeeded.\r\n");
    }
    else
    {
        printf("SPI communication failed.\r\n");
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
static uint8_t BufferCompare(uint32_t *buf1, uint32_t *buf2, uint8_t size)
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

/**
 * @brief   DMA channel 0 interrupt service routine
 *
 * @param   None
 *
 * @retval  None
 */
void DMA_CH0_ISR(void)
{
    if (DDL_DMA_IsActiveFlag_TC0(DMA))
    {
        DMA_CH0_TC_IntRequest();
    }
}

/**
 * @brief   DMA channel 1 interrupt service routine
 *
 * @param   None
 *
 * @retval  None
 */
void DMA_CH1_ISR(void)
{
    if (DDL_DMA_IsActiveFlag_TC1(DMA))
    {
        DMA_CH1_TC_IntRequest();
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

/**
 * @brief   DMA CH0 TC interrupt request function
 *
 * @param   None
 *
 * @retval  None
 */
static void DMA_CH0_TC_IntRequest(void)
{
    DDL_DMA_ClearFlag_TC0(DMA);
}

/**
 * @brief   DMA CH1 TC interrupt request function
 *
 * @param   None
 *
 * @retval  None
 */
static void DMA_CH1_TC_IntRequest(void)
{
    DDL_DMA_ClearFlag_TC1(DMA);
    tcFlag = 1;
}
