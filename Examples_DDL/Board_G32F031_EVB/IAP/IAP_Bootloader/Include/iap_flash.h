/**
 * @file        iap_flash.h
 *
 * @brief       Header for iap_flash.c module
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

/* Define to prevent recursive inclusion */
#ifndef IAP_FLASH_H
#define IAP_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "g32f031_device_cfg.h"

/* Exported macro *********************************************************/
#define IAP_FLASH_BASE ((uint32_t)0x00000000U)

#define IAP_FLASH_PAGE_SIZE ((uint16_t)0x200)

#define USER_APP1_START_ADDRESS (IAP_FLASH_BASE + 0x4000U)
/* 8 KBytes */
#define USER_APP1_END_ADDRESS   (IAP_FLASH_BASE + 0x5FFFU)
#define USER_APP1_FLASH_SIZE    (USER_APP1_END_ADDRESS - USER_APP1_START_ADDRESS + 1)

#define USER_APP2_START_ADDRESS (IAP_FLASH_BASE + 0x6000U)
/* 8 KBytes */
#define USER_APP2_END_ADDRESS   (IAP_FLASH_BASE + 0x7FFFU)
#define USER_APP2_FLASH_SIZE    (USER_APP2_END_ADDRESS - USER_APP2_START_ADDRESS + 1)

#define USER_FLASH_SIZE (USER_APP2_END_ADDRESS - USER_APP1_START_ADDRESS)

/* Exported typedef *******************************************************/

/**
 * @brief   APP type define
 */
typedef enum
{
    IAP_APP1, /*!< Application1 */
    IAP_APP2  /*!< Application2 */
} IAP_APP_T;

/**
 * @brief    function hook
 */
typedef void (*pFunction)(void);

/* Exported function prototypes *******************************************/
void IAP_FLASH_Init(void);
uint32_t IAP_FLASH_Erase(IAP_APP_T application);
uint32_t IAP_FLASH_Write(uint32_t address, uint8_t *data, uint16_t length, IAP_APP_T application);
void IAP_Jump2App(IAP_APP_T application);
uint16_t IAP_OPT_ReadWriteProtect(void);

#ifdef __cplusplus
}
#endif

#endif /* IAP_FLASH_H */
