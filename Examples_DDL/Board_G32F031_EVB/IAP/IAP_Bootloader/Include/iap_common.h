/*!
 * @file        iap_common.h
 *
 * @brief       Header for iap_common.c module
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
#ifndef IAP_COMMON_H
#define IAP_COMMON_H

/* Includes ***************************************************************/
#include "g32f031_device_cfg.h"
#include "iap_flash.h"

/* Exported macro *********************************************************/
#define UART_TX_TIMEOUT ((uint32_t)100U)
#define UART_RX_TIMEOUT ((uint32_t)5000U)

#define CHECK_AF(s)  ((s >= 'A') && (s <= 'F'))
#define CHECK_af(s)  ((s >= 'a') && (s <= 'f'))
#define CHECK_09(s)  ((s >= '0') && (s <= '9'))
#define CHECK_HEX(s) CHECK_AF(s) || CHECK_af(s) || CHECK_09(s)
#define CHECK_DEC(s) CHECK_09(s)
#define CON_DEC(s)   (s - '0')

#define CON_HEX_alpha(s) (CHECK_AF(s) ? (s - 'A' + 10) : (s - 'a' + 10))
#define CON_HEX(s)       (CHECK_09(s) ? (s - '0') : CON_HEX_alpha(s))

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
void IntConStr(uint8_t *str, uint32_t conint);
uint32_t StrConInt(uint8_t *str, uint32_t *conint);

void Serial_SendString(uint8_t *str);
ErrorStatus Serial_SendByte(uint8_t data);
ErrorStatus Receive_Byte(uint8_t *data, uint32_t revTimeout);

#endif /* IAP_COMMON_H */
