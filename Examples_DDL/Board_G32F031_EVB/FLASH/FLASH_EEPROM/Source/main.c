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

/* Private includes *******************************************************/
#include "g32f031_device_cfg.h"
#include "flash_eeprom.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Read/Write buffer size, The value must be less than the NUMBER_OF_VARIABLES macro */
#define BUFFER_SIZE 16

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* read and write buffer */
uint32_t Write_Buffer[BUFFER_SIZE] = {0};
uint32_t Read_Buffer[BUFFER_SIZE] = {0};

/* Private function prototypes ********************************************/
void PrintBuffer(uint32_t *pBuffer, uint32_t BufferLength);
uint8_t BufferCompare(uint32_t *Buff1, uint32_t *Buff2, uint32_t Size);

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
    uint32_t i, VirtAddress;

    /* Device configuration */
    Device_Config();

    /* fill Write_Buffer */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        Write_Buffer[i] = i + 0x100;
    }

    /* Print write data */
    printf("write data:\r\n");
    PrintBuffer(Write_Buffer, BUFFER_SIZE);

    /* Flash EEPROM Init */
    Flash_EE_Init();

    /* Write the variable data and corresponding virtual address to Flash EEPROM */
    /* Virtual address value must be between 0 ~ NUMBER_OF_VARIABLES */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        VirtAddress = i;
        Flash_EE_WriteData(VirtAddress, Write_Buffer[i]);
    }

    /* Read the variable data corresponding to the virtual address */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        VirtAddress = i;
        Flash_EE_ReadData(VirtAddress, &Read_Buffer[i]);
    }

    /* Print read data */
    printf("read data:\r\n");
    PrintBuffer(Read_Buffer, BUFFER_SIZE);

    /* Compare the values of two buffers for equality */
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0)
    {
        BOARD_LED_On(LED2);
        printf("Flash EEPROM test successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Flash EEPROM test fail!\r\n");
    }

    /* Infinite loop */
    while (1)
    {
    }
}

/*!
 * @brief       Print Buffer Data
 *
 * @param       pBuffer: buffer
 *
 * @param       length : length of the Buffer
 *
 * @retval      None
 */
void PrintBuffer(uint32_t *pBuffer, uint32_t BufferLength)
{
    uint32_t i = 0;

    for (i = 0; i < BufferLength; i++)
    {
        printf(" 0x%lx", pBuffer[i]);

        if ((i + 1) % 8 == 0)
        {
            printf("\r\n");
        }
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
uint8_t BufferCompare(uint32_t *Buff1, uint32_t *Buff2, uint32_t Size)
{
    uint32_t i = 0;

    for (i = 0; i < Size; i++)
    {
        if (Buff1[i] != Buff2[i])
        {
            return 0;
        }
    }

    return 1;
}
