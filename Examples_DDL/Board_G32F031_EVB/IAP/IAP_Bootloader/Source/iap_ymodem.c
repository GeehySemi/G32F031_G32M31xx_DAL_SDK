/**
 * @file        iap_ymodem.c
 *
 * @brief       Include Ymodem operation to HyperTerminal
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
#include "iap_ymodem.h"

/* Private includes *******************************************************/
#include "iap_common.h"
#include "iap_menu.h"
#include "g32f031_device_cfg.h"
#include <string.h>

/* Private macro **********************************************************/
#define CRC16_F

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint8_t filek[PACKET_1K_SIZE + PACK_DATA_INDEX + PACK_FRAME_TRAILER];

/* Private function prototypes ********************************************/
static uint16_t CRC16_Update(uint16_t value, uint8_t b);
static uint16_t CRC16_Sum(const uint8_t *data, uint32_t len);
static uint8_t Check_sum(const uint8_t *data, uint32_t len);
static void PreFirstPack(uint8_t *data, const uint8_t *fileName, uint32_t *length);
static void PrePack(uint8_t *sourceBuf, uint8_t *data, uint8_t packNo, uint32_t lenBlk);
static int32_t Ymodem_ReceivePacket(uint8_t *data, uint32_t *len, uint32_t timeout);

/* External variables *****************************************************/
extern uint8_t fileN[];

/* External functions *****************************************************/

/**
 * @brief   Receive a file from sender using the Ymodem
 *
 * @param   size: size of file
 *
 * @param   application : IAP_APP1 or IAP_APP2
 *
 * @retval  Ymodem status
 */
YMODEM_STA_T Ymodem_RecceiveFile(uint32_t *size, IAP_APP_T application)
{
    YMODEM_STA_T result = YMODEM_OK;
    __IO uint32_t flashAddress;
    __IO uint32_t userFlashSize;

    uint32_t i, packLen, fileFinish;
    uint32_t errors = 0U;
    uint32_t sessionFinish = 0U, sessionBegin = 0U;

    uint32_t ramSource, fileSize, packsReceived;
    uint8_t *filePtr;
    uint8_t fileSizeArr[FILE_SIZE_MAX];
    uint8_t temp;

    /* Init flashAddress */
    if (application == IAP_APP1)
    {
        flashAddress = USER_APP1_START_ADDRESS;
        userFlashSize = USER_APP1_FLASH_SIZE;
    }
    else
    {
        flashAddress = USER_APP2_START_ADDRESS;
        userFlashSize = USER_APP2_FLASH_SIZE;
    }

    while ((sessionFinish == 0U) && (result == YMODEM_OK))
    {
        packsReceived = 0U;
        fileFinish = 0U;
        while ((fileFinish == 0U) && (result == YMODEM_OK))
        {
            switch (Ymodem_ReceivePacket(filek, &packLen, DOWNLOAD_TIMEOUT))
            {
                case 0:
                    errors = 0U;

                    switch (packLen)
                    {
                        case 2U:
                            /* Abort by sender */
                            Serial_SendByte(FRAME_ACK);
                            result = YMODEM_ABORT;
                            break;

                        case 0U:
                            /* End of transmission */
                            Serial_SendByte(FRAME_ACK);
                            fileFinish = 1;
                            break;

                        default:
                            /* Normal packet */
                            if (filek[PACK_NUMBER_INDEX] != (uint8_t)packsReceived)
                            {
                                Serial_SendByte(FRAME_NACK);
                            }
                            else
                            {
                                if (packsReceived == 0U)
                                {
                                    /* File name packet */
                                    if (filek[PACK_DATA_INDEX] != 0U)
                                    {
                                        /* File name extraction */
                                        i = 0U;
                                        filePtr = filek + PACK_DATA_INDEX;
                                        while ((*filePtr != 0U) && (i < FILE_NAME_MAX))
                                        {
                                            fileN[i++] = *filePtr++;
                                        }

                                        /* File size extraction */
                                        fileN[i++] = '\0';
                                        i = 0U;
                                        filePtr++;
                                        while ((*filePtr != ' ') && (i < FILE_SIZE_MAX))
                                        {
                                            fileSizeArr[i++] = *filePtr++;
                                        }
                                        fileSizeArr[i++] = '\0';
                                        StrConInt(fileSizeArr, &fileSize);

                                        /* Test the size of the image to be sent */
                                        /* Image size is greater than Flash size */
                                        if (fileSize > (userFlashSize + 1U))
                                        {
                                            /* End session */
                                            temp = FRAME_CAN;
                                            DDL_USART_TransmitData8(USART, temp);
                                            while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
                                                ;
                                            DDL_USART_TransmitData8(USART, temp);
                                            while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
                                                ;
                                            result = YMODEM_LIMIT;
                                        }

                                        /* Erase user application area */
                                        IAP_FLASH_Erase(application);
                                        *size = fileSize;

                                        Serial_SendByte(FRAME_ACK);
                                        Serial_SendByte(FRAME_CRC16);
                                    }
                                    /* File header packet is empty, end session */
                                    else
                                    {
                                        Serial_SendByte(FRAME_ACK);
                                        fileFinish = 1U;
                                        sessionFinish = 1U;
                                        break;
                                    }
                                }
                                /* Data packet */
                                else
                                {
                                    ramSource = (uint32_t)&filek[PACK_DATA_INDEX];
                                    /* Write received data in Flash */
                                    if (IAP_FLASH_Write(flashAddress,
                                                        (uint8_t *)ramSource,
                                                        packLen,
                                                        application) == SUCCESS)
                                    {
                                        flashAddress += packLen;
                                        Serial_SendByte(FRAME_ACK);
                                    }
                                    else
                                    {
                                        /* End session */
                                        Serial_SendByte(FRAME_CAN);
                                        Serial_SendByte(FRAME_CAN);
                                        result = YMODEM_DATA;
                                    }
                                }
                                packsReceived++;
                                sessionBegin = 1U;
                            }
                            break;
                    }
                    break;

                /* Abort actually */
                case 1:
                    Serial_SendByte(FRAME_CAN);
                    Serial_SendByte(FRAME_CAN);
                    result = YMODEM_ABORT;
                    break;

                default:
                    if (sessionBegin > 0U)
                    {
                        errors++;
                    }
                    if (errors > MAX_ERRORS)
                    {
                        /* Abort communication */
                        Serial_SendByte(FRAME_CAN);
                        Serial_SendByte(FRAME_CAN);
                    }
                    else
                    {
                        /* Ask for a packet */
                        Serial_SendByte(FRAME_CRC16);
                    }
                    break;
            }
        }
    }

    return result;
}

/**
 * @brief   Transmit a file using the Ymodem
 *
 * @param   buf: File address of the first byte
 *
 * @param   name: File name
 *
 * @param   lenFile: The lenght of the file
 *
 * @retval  Ymodem status
 */
YMODEM_STA_T Ymodem_TransmitFile(uint8_t *buf, const uint8_t *name, uint32_t lenFile)
{
    uint8_t FileN[FILE_NAME_MAX];
    uint8_t *buf_p;
    uint8_t checkSumTemp;
    uint16_t crcTemp;
    uint16_t blockNum;
    uint8_t revC[2];
    uint8_t crc16F = 0U;
    uint8_t i = 0U;
    uint32_t err;
    uint32_t len = 0U;
    uint32_t packSize;

    while ((i < (FILE_NAME_MAX - 1U)) && (name[i] != '\0'))
    {
        FileN[i] = name[i];
        i++;
    }
    FileN[i] = '\0';

    crc16F = 1U;

    /* Prepare the first packet */
    PreFirstPack(&filek[0], FileN, &lenFile);

    for (err = 0; err < MAX_ERRORS;)
    {
        /* Send Packet to receiver*/
        for (int i = 0; i < PACKET_SIZE + PACK_FRAME_HEADER; i++)
        {
            DDL_USART_TransmitData8(USART, filek[i]);
            while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
            {
            }
        }

        /* Send CRC or checksum based on crc16F */
        if (crc16F)
        {
            crcTemp = CRC16_Sum(&filek[3], PACKET_SIZE);
            Serial_SendByte(crcTemp >> 8U);
            Serial_SendByte(crcTemp & 0xFFU);
        }
        else
        {
            checkSumTemp = Check_sum(&filek[3], PACKET_SIZE);
            Serial_SendByte(checkSumTemp);
        }

        /* Wait for ACK */
        if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
        {
            err++;
            continue;
        }

        if (revC[0] == FRAME_ACK)
        {
            /* Receiver requests the first data packet after ACKing block 0. */
            if ((Receive_Byte(&revC[1], DOWNLOAD_TIMEOUT) == SUCCESS) &&
                (revC[1] == FRAME_CRC16))
            {
                break;
            }
        }

        err++;
    }

    if (err >= MAX_ERRORS)
    {
        return YMODEM_ERROR;
    }

    buf_p = buf;
    len = lenFile;
    blockNum = 0x01U;

    /* 1024 bytes package */
    while (len)
    {
        /* Prepare pack */
        PrePack(buf_p, &filek[0], blockNum, len);
        revC[0] = 0U;
        err = 0U;

        for (err = 0U; err < MAX_ERRORS;)
        {
            /* Send pack */
            if (len >= PACKET_1K_SIZE)
            {
                packSize = PACKET_1K_SIZE;
            }
            else
            {
                packSize = PACKET_SIZE;
            }

            for (int i = 0; i < packSize + PACK_FRAME_HEADER; i++)
            {
                DDL_USART_TransmitData8(USART, filek[i]);
                while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
                {
                }
            }

            /* Send CRC or checksum based on crc16F */
            if (crc16F)
            {
                crcTemp = CRC16_Sum(&filek[3], packSize);
                Serial_SendByte(crcTemp >> 8U);
                Serial_SendByte(crcTemp & 0xFFU);
            }
            else
            {
                checkSumTemp = Check_sum(&filek[3], packSize);
                Serial_SendByte(checkSumTemp);
            }

            /* Wait for ACK */
            if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
            {
                err++;
                continue;
            }

            if (revC[0] == FRAME_ACK)
            {
                if (len > packSize)
                {
                    buf_p += packSize;
                    len -= packSize;

                    if (blockNum == (USER_FLASH_SIZE / 1024U))
                    {
                        /* error */
                        return YMODEM_ERROR;
                    }
                    else
                    {
                        blockNum++;
                    }
                }
                else
                {
                    buf_p += packSize;
                    len = 0U;
                }

                break;
            }
            else
            {
                err++;
            }
        }

        /* Resend pack */
        if (err >= MAX_ERRORS)
        {
            return YMODEM_ERROR;
        }
    }

    /* First EOT must be answered with NAK. */
    for (err = 0U; err < MAX_ERRORS;)
    {
        Serial_SendByte(FRAME_EOT);

        if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
        {
            err++;
            continue;
        }

        if (revC[0] == FRAME_NACK)
        {
            break;
        }

        err++;
    }

    if (err >= MAX_ERRORS)
    {
        return YMODEM_ERROR;
    }

    /* Second EOT must be answered with ACK. */
    for (err = 0U; err < MAX_ERRORS;)
    {
        Serial_SendByte(FRAME_EOT);

        if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
        {
            err++;
            continue;
        }

        if (revC[0] == FRAME_ACK)
        {
            break;
        }

        err++;
    }

    if (err >= MAX_ERRORS)
    {
        return YMODEM_ERROR;
    }

    /* Receiver requests the final empty block 0. */
    for (err = 0U; err < MAX_ERRORS;)
    {
        if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
        {
            err++;
            continue;
        }

        if (revC[0] == FRAME_CRC16)
        {
            break;
        }

        err++;
    }

    if (err >= MAX_ERRORS)
    {
        return YMODEM_ERROR;
    }

    filek[0] = FRAME_SOH;
    filek[1] = 0U;
    filek[2] = 0xFFU;

    for (i = PACK_FRAME_HEADER; i < (PACKET_SIZE + PACK_FRAME_HEADER); i++)
    {
        filek[i] = 0x00U;
    }

    for (err = 0U; err < MAX_ERRORS;)
    {
        for (int i = 0; i < PACKET_SIZE + PACK_FRAME_HEADER; i++)
        {
            DDL_USART_TransmitData8(USART, filek[i]);
            while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
            {
            }
        }

        crcTemp = CRC16_Sum(&filek[PACK_DATA_INDEX], PACKET_SIZE);
        Serial_SendByte(crcTemp >> 8U);
        Serial_SendByte(crcTemp & 0xFFU);

        if (Receive_Byte(&revC[0], DOWNLOAD_TIMEOUT) != SUCCESS)
        {
            err++;
            continue;
        }

        if (revC[0] == FRAME_ACK)
        {
            return YMODEM_OK;
        }

        err++;
    }

    return YMODEM_ERROR;
}

/**
 * @brief   Update CRC16 for input byte
 *
 * @param   value: CRC input value
 *
 * @param   b: input byte
 *
 * @retval  returnTemp: Updated CRC value
 *
 * @note
 */
static uint16_t CRC16_Update(uint16_t value, uint8_t b)
{
    uint32_t value_crc = value;
    uint32_t value_int = b | 0x100;
    uint16_t returnTemp;

    do
    {
        value_crc <<= 1;
        value_int <<= 1;

        if (value_int & 0x100)
        {
            ++value_crc;
        }

        if (value_crc & 0x10000)
        {
            value_crc ^= 0x1021;
        }
    } while (!(value_int & 0x10000));

    returnTemp = (uint16_t)(value_crc & 0xffff);

    return returnTemp;
}

/**
 * @brief   Cal CRC16
 *
 * @param   data
 *
 * @param   len
 *
 * @retval  CRC16 sum
 */
static uint16_t CRC16_Sum(const uint8_t *data, uint32_t len)
{
    uint32_t crc_sum = 0;
    const uint8_t *data_len = data + len;

    do
    {
        crc_sum = CRC16_Update(crc_sum, *data++);
    } while (data < data_len);

    crc_sum = CRC16_Update(CRC16_Update(crc_sum, 0), 0);

    crc_sum &= (uint16_t)0xffff;

    return crc_sum;
}

/**
 * @brief   Cal Check sum for YModem Packet
 *
 * @param   data
 *
 * @param   len
 *
 * @retval  checksum
 */
static uint8_t Check_sum(const uint8_t *data, uint32_t len)
{
    uint32_t checksum = 0;
    const uint8_t *data_len = data + len;

    do
    {
        checksum += *data++;
    } while (data < data_len);

    checksum &= (uint16_t)0xff;

    return checksum;
}

/**
 * @brief   Receive a Ymodem packet from sender
 *
 * @param   data
 *
 * @param   len
 *
 * @param   timeout
 *
 * @retval  receive status
 *          @arg 0 : success
 *          @arg -1: timeout or error
 *          @arg 1 : abort
 *
 */
static int32_t Ymodem_ReceivePacket(uint8_t *data, uint32_t *len, uint32_t timeout)
{
    int32_t status = 0;
    uint32_t pack_crc;
    uint32_t pack_len = 0;
    uint8_t character;
    *len = 0;
    if (Receive_Byte(&character, timeout) != SUCCESS)
    {
        return -1;
    }

    switch (character)
    {
        case FRAME_SOH:
            pack_len = PACKET_SIZE;
            break;

        case FRAME_STX:
            pack_len = PACKET_1K_SIZE;
            break;

        case FRAME_EOT:
            break;

        case FRAME_CAN:
            if (Receive_Byte(&character, timeout) == SUCCESS && (character == FRAME_CAN))
            {
                *len = 0;
                return 0;
            }
            else
            {
                return -1;
            }

        case ABORT1:
        case ABORT2:
            status = 1;
            break;

        default:
            status = -1;
            break;
    }

    *data = character;

    if (pack_len >= PACKET_SIZE)
    {
        for (int i = 1; i < (pack_len + PACK_FRAME_OVERHEAD); i++)
        {
            if (Receive_Byte(data + i, timeout) != SUCCESS)
            {
                *len = 0U;
                return -1;
            }
        }

        /* Check packet sanity */
        if (data[PACK_NUMBER_INDEX] != ((data[PACK_CNUMBER_INDEX]) ^ FRAME_NEGATIVE))
        {
            pack_len = 0;
            status = -1;
        }
        else
        {
            /* Check packet CRC */
            pack_crc = data[pack_len + PACK_DATA_INDEX] << 8;
            pack_crc += data[pack_len + PACK_DATA_INDEX + 1];
            if (CRC16_Sum(&data[PACK_DATA_INDEX], pack_len) != pack_crc)
            {
                pack_len = 0;
                status = -1;
            }
        }
    }

    *len = pack_len;

    return status;
}

/**
 * @brief   Prepare the Ymodem first packet
 *
 * @param   data: packet data
 *
 * @param   fileName: file name
 *
 * @param   length: file lenght
 *
 * @retval  None
 */
static void PreFirstPack(uint8_t *data, const uint8_t *fileName, uint32_t *length)
{
    uint16_t i, j;
    uint8_t file_p[11];

    /* Make frame three pack */
    data[0] = FRAME_SOH;
    data[1] = 0x00;
    data[2] = 0xff;

    i = 0;

    do
    {
        data[i + PACK_FRAME_HEADER] = fileName[i];
        i++;
    } while ((fileName[i] != '\0') && (i < FILE_NAME_MAX));

    data[i + PACK_FRAME_HEADER] = 0x00;

    IntConStr(file_p, *length);

    i = i + PACK_FRAME_HEADER + 1;
    j = 0;

    do
    {
        data[i++] = file_p[j++];
    } while (file_p[j] != '\0');

    j = i;

    do
    {
        data[j] = 0;
        j++;
    } while (j < PACKET_SIZE + PACK_FRAME_HEADER);
}

/**
 * @brief   Prepare the Ymodem data pack
 *
 * @param   sourceBuf:source buffer
 *
 * @param   data:packet data
 *
 * @param   packNo:packet block number
 *
 * @param   lenBlk: packet block lenght
 *
 * @retval  None
 */
static void PrePack(uint8_t *sourceBuf, uint8_t *data, uint8_t packNo, uint32_t lenBlk)
{
    uint16_t i, len, packSize;
    uint8_t *file_p;

    /* Make frame three pack */
    if (lenBlk < PACKET_1K_SIZE)
    {
        packSize = PACKET_SIZE;
    }
    else
    {
        packSize = PACKET_1K_SIZE;
    }

    if (lenBlk >= packSize)
    {
        len = packSize;
    }
    else
    {
        len = lenBlk;
    }

    if (packSize != PACKET_1K_SIZE)
    {

        data[0] = FRAME_SOH;
    }
    else
    {
        data[0] = FRAME_STX;
    }

    data[1] = packNo;
    data[2] = (~packNo);
    file_p = sourceBuf;

    for (i = PACK_FRAME_HEADER; i < len + PACK_FRAME_HEADER; i++)
    {
        data[i] = *file_p++;
    }

    if (len <= packSize)
    {
        for (i = len + PACK_FRAME_HEADER; i < packSize + PACK_FRAME_HEADER; i++)
        {
            data[i] = FILE_FILL_VAL;
        }
    }
}
