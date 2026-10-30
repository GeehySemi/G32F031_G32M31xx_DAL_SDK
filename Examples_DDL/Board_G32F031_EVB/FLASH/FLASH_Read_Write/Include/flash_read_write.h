/*!
 * @file        flash_read_write.h
 *
 * @brief       Header for flash_read_write.c module
 *
 * @version     V1.0.0
 *
 * @date        2025-06-01
 *
 * @attention
 *
 *  Copyright (C) 2025 Geehy Semiconductor
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

/* The following is the Flash region planning for this example. */

/*
 +--------------------------------------------------------------+
 |                             Flash                            |
 +-------------------------------+------------------------------+
 |  Code  |      User Para area      |           Code           |
 +--------+--------+--------+--------+--------+-----------------+
 |  15KB  |  512B  |  512B  |  512B  |  512B  |  512B  |  512B  |
 | sector | sector | sector | sector | sector | sector | sector |
 |   30   |   31   |   32   |   33   |   34   |   N    | N + 1  |
 |        |        |        |        |        |        |        |
 +--------+--------+--------+--------+--------+-----------------+
*/
/* Exported macro *********************************************************/

/* flash sector satrt address */
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x00006000) /* 512 bytes */
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x00006200) /* 512 bytes */
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x00006400) /* 512 bytes */

/*  flash start address */
#define G32_FLASH_START_ADDR ((uint32_t)(0x00000000))
/*  flash size */
#define G32_FLASH_SIZE       (64 * 1024)
/*  flash end address */
#define G32_FLASH_END_ADDR   ((uint32_t)(G32_FLASH_START_ADDR + G32_FLASH_SIZE))
/*  flash sector/page size */
#define G32_FLASH_PAGE_SIZE  ((uint32_t)(0x200))

/* flash read write total size. This value must be a multiple of the page size. */
#define FLASH_READ_WRITE_TOTAL_SIZE ((uint32_t)(G32_FLASH_PAGE_SIZE * 2))

/* flash read write page start address, it's must be page aligned */
#define FLASH_READ_WRITE_START_ADDR ADDR_FLASH_SECTOR_1

/* flash read write page start address */
#define FLASH_READ_WRITE_END_ADDR (ADDR_FLASH_SECTOR_1 + FLASH_READ_WRITE_TOTAL_SIZE)

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/

/* Read the specified length of data from the specified address */
int Flash_Read(uint32_t readAddr, uint8_t *pData, uint32_t len);
/* Write the specified length of data from the specified address */
int Flash_Write(uint32_t writeAddr, uint8_t *pData, uint32_t len);
