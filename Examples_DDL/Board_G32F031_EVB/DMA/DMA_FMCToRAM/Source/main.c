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
#define DMA_BUFFER_SIZE 64U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
static const uint32_t srcBuffer[DMA_BUFFER_SIZE] = {
    0x01020304U, 0x05060708U, 0x090A0B0CU, 0x0D0E0F10U, 0x11121314U, 0x15161718U, 0x191A1B1CU,
    0x1D1E1F20U, 0x21222324U, 0x25262728U, 0x292A2B2CU, 0x2D2E2F30U, 0x31323334U, 0x35363738U,
    0x393A3B3CU, 0x3D3E3F40U, 0x41424344U, 0x45464748U, 0x494A4B4CU, 0x4D4E4F50U, 0x51525354U,
    0x55565758U, 0x595A5B5CU, 0x5D5E5F60U, 0x61626364U, 0x65666768U, 0x696A6B6CU, 0x6D6E6F70U,
    0x71727374U, 0x75767778U, 0x797A7B7CU, 0x7D7E7F80U,

    0x81828384U, 0x85868788U, 0x898A8B8CU, 0x8D8E8F90U, 0x91929394U, 0x95969798U, 0x999A9B9CU,
    0x9D9E9FA0U, 0xA1A2A3A4U, 0xA5A6A7A8U, 0xA9AAABACU, 0xADAEAFB0U, 0xB1B2B3B4U, 0xB5B6B7B8U,
    0xB9BABBBCU, 0xBDBEBFC0U, 0xC1C2C3C4U, 0xC5C6C7C8U, 0xC9CACBCCU, 0xCDCECFD0U, 0xD1D2D3D4U,
    0xD5D6D7D8U, 0xD9DADBDCU, 0xDDDEDFE0U, 0xE1E2E3E4U, 0xE5E6E7E8U, 0xE9EAEBECU, 0xEDEEEFF0U,
    0xF1F2F3F4U, 0xF5F6F7F8U, 0xF9FAFBFCU, 0xFDFEFF00U,
};

static uint32_t dstBuffer[DMA_BUFFER_SIZE];

/* Private function prototypes ********************************************/

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

    DDL_DMA_SetM2MSrcAddress(DMA, DDL_DMA_CHANNEL_1, (uint32_t)srcBuffer);
    DDL_DMA_SetM2MDstAddress(DMA, DDL_DMA_CHANNEL_1, (uint32_t)dstBuffer);
    DDL_DMA_SetDataLength(DMA, DDL_DMA_CHANNEL_1, DMA_BUFFER_SIZE);

    DDL_DMA_EnableChannel(DMA, DDL_DMA_CHANNEL_1);
    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        DDL_mDelay(500U);
    }
}
/**
 * @brief   DMA interrupt handler
 *
 * @param   None
 *
 * @retval  None
 */
void DMA_ISR(void)
{
    if (DDL_DMA_IsActiveFlag_TC1(DMA))
    {
        DDL_DMA_ClearFlag_TC1(DMA);
        printf("DMA Transfer Complete\r\n");
    }

    if (DDL_DMA_IsActiveFlag_DME1(DMA))
    {
        DDL_DMA_ClearFlag_DME1(DMA);
        printf("DMA Transfer Error\r\n");
    }
}
