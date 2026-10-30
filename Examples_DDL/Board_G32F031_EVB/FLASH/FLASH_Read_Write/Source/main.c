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
#include "flash_read_write.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/
/* Flash read write test address */
#define FLASH_READ_WRITE_TEST_ADDR (FLASH_READ_WRITE_START_ADDR + G32_FLASH_PAGE_SIZE)

/* test buffer size */
#define BUFFER_SIZE 64

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
/* read and write buffer */
static uint8_t Write_Buffer[BUFFER_SIZE] = {0};
static uint8_t Read_Buffer[BUFFER_SIZE] = {0};

/* Private function prototypes ********************************************/
void InitBuffer(void);
uint8_t BufferCompare(uint8_t *buf1, uint8_t *buf2, uint32_t size);

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
    uint32_t addr = 0U;

    /* Device configuration */
    Device_Config();

    /* Page aligned write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Page aligned write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Page aligned write operation fail!\r\n");
    }

    /* Page non-aligned write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR + 16;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Page non-aligned write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Page non-aligned write operation fail!\r\n");
    }

    /* Cross page write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR - 8;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Cross page write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Cross page write operation fail!\r\n");
    }

    /* Infinite loop */
    while (1)
    {
    }
}

/*!
 * @brief       Initialize Flash read write test buffers
 *
 * @param       None
 *
 * @retval      None
 */
void InitBuffer(void)
{
    uint32_t idx = 0U;
    for (idx = 0U; idx < BUFFER_SIZE; idx++)
    {
        Write_Buffer[idx] = (uint8_t)idx;
        Read_Buffer[idx] = 0U;
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
uint8_t BufferCompare(uint8_t *buf1, uint8_t *buf2, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return 0;
        }
    }

    return 1;
}
