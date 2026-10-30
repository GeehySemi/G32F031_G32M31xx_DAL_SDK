/*!
 * @file        ymodem.h
 *
 * @brief       Header for ymodem.c module
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
#ifndef IAP_YMODEM_H
#define IAP_YMODEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "g32f031_device_cfg.h"
#include "iap_flash.h"

/* Exported macro *********************************************************/
#define PACK_DATA_INDEX             (3)
#define PACK_CNUMBER_INDEX          (2)
#define PACK_NUMBER_INDEX           (1)
#define PACK_START_INDEX            (0)
#define PACK_FRAME_HEADER           (3)
#define PACK_FRAME_TRAILER          (2)
#define PACK_FRAME_OVERHEAD         (PACK_FRAME_HEADER + PACK_FRAME_TRAILER)
#define PACKET_SIZE                 (128)
#define PACKET_1K_SIZE              (1024)
#define PACK_1K_FRAME_OVERHEAD_SIZE (PACKET_1K_SIZE + PACK_FRAME_OVERHEAD)

#define FILE_NAME_MAX (64)
#define FILE_SIZE_MAX (16)

#define FILE_FILL_VAL (0x1A)

#define FRAME_SOH      (0x01) /*!< start of 128-byte data packet */
#define FRAME_STX      (0x02) /*!< start of 1024-byte data packet */
#define FRAME_EOT      (0x04) /*!< end of transmission */
#define FRAME_ACK      (0x06) /*!< acknowledge */
#define FRAME_NACK     (0x15) /*!< negative acknowledge */
#define FRAME_CAN      (0x18) /*!< cancel transmission */
#define FRAME_CRC16    (0x43) /*!< 'C' == 0x43, request 16-bit CRC */
#define FRAME_NEGATIVE (0xFF)

#define ABORT1 (0x41) /*!< 'A' == 0x41, abort by user */
#define ABORT2 (0x61) /*!< 'a' == 0x61, abort by user */

#define NAK_TIMEOUT      (0x100000)
#define DOWNLOAD_TIMEOUT (0x100000)
#define MAX_ERRORS       (10)

/* Exported typedef *******************************************************/

/**
 * @brief  Ymodem communication status structures definition
 */
typedef enum
{
    YMODEM_OK,
    YMODEM_ERROR,
    YMODEM_ABORT,
    YMODEM_TIMEOUT,
    YMODEM_DATA,
    YMODEM_LIMIT,
} YMODEM_STA_T;

/* Exported function prototypes *******************************************/
YMODEM_STA_T Ymodem_RecceiveFile(uint32_t *size, IAP_APP_T application);
YMODEM_STA_T Ymodem_TransmitFile(uint8_t *buf, const uint8_t *name, uint32_t lenFile);

#ifdef __cplusplus
}
#endif

#endif /* IAP_YMODEM_H */
