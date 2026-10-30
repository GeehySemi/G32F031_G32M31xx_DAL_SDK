/**
 * @file        iap_menu.c
 *
 * @brief       Display a usart menu to user and include IAP download and upload and area jump
 * operation
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
#include "iap_menu.h"

/* Private includes *******************************************************/
#include "iap_common.h"
#include "iap_flash.h"
#include "iap_ymodem.h"
#include "g32f031_device_cfg.h"

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint8_t fileN[FILE_NAME_MAX];

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* External functions *****************************************************/

/**
 * @brief   Download a file via serial port
 *
 * @param   application
 *          @arg IAP_APP1 : IAP_Application1
 *          @arg IAP_APP2 : IAP_Application2
 *
 * @retval  None
 */
void Download(IAP_APP_T application)
{
    uint8_t number[11] = {0}, i = 0;
    uint32_t size = 0U;
    YMODEM_STA_T status = YMODEM_OK;

    Serial_SendString((uint8_t *)">> Waiting for the file to be sent ... (press 'a' to abort)\r\n");
    status = Ymodem_RecceiveFile(&size, application);

    switch (status)
    {
        case YMODEM_ABORT:
            Serial_SendString((uint8_t *)">> Operation aborted!.\r\n");
            break;

        case YMODEM_DATA:
            Serial_SendString((uint8_t *)">> Verification failed!\r\n");
            break;

        case YMODEM_LIMIT:
            Serial_SendString((uint8_t *)">> The file is too large!\r\n");
            break;

        case YMODEM_OK:
            Serial_SendString((uint8_t *)">> Programming Completed Successfully!\r\n");

            Serial_SendString((uint8_t *)"***********");

            for (i = 0; (i < 255) && (fileN[i] != 0x00); i++)
            {
                Serial_SendString((uint8_t *)"*");
            }

            Serial_SendString((uint8_t *)"\n\r   Name: ");
            Serial_SendString(fileN);
            IntConStr(number, size);
            Serial_SendString((uint8_t *)"\n\r   Size: ");
            Serial_SendString(number);
            Serial_SendString((uint8_t *)" Bytes\r\n");

            Serial_SendString((uint8_t *)"***********");

            for (i = 0; (i < 255) && (fileN[i] != 0x00); i++)
            {
                Serial_SendString((uint8_t *)"*");
            }
            break;

        default:
            Serial_SendString((uint8_t *)"Receive File is Failed\r\n");
            break;
    }
}

/**
 * @brief   Upload a file via serial port.
 *
 * @param   application
 *          @arg IAP_APP1 : IAP_Application1
 *          @arg IAP_APP2 : IAP_Application2
 *
 * @retval  None
 *
 * @note
 */
void Upload(IAP_APP_T application)
{
    uint8_t frameStatus;
    YMODEM_STA_T status = YMODEM_OK;

    uint32_t timeout = 0xFFFF;

    Serial_SendString((uint8_t *)">> Select Receive File\n\r");

    while (DDL_USART_IsActiveFlag_RXNE(USART) == 0)
        ;
    frameStatus = DDL_USART_ReceiveData8(USART);

    if (frameStatus == FRAME_CRC16)
    {

        if (application == IAP_APP1)
        {
            /** Transmit the flash image through ymodem protocol */
            status = Ymodem_TransmitFile((uint8_t *)USER_APP1_START_ADDRESS,
                                         (const uint8_t *)"Up_G32F031_App1_FlashImage.bin",
                                         USER_APP1_FLASH_SIZE);
        }
        else
        {
            /** Transmit the flash image through ymodem protocol */
            status = Ymodem_TransmitFile((uint8_t *)USER_APP2_START_ADDRESS,
                                         (const uint8_t *)"Up_G32F031_App2_FlashImage.bin",
                                         USER_APP2_FLASH_SIZE);
        }

        while (timeout-- > 0)
        {
        }

        if (status == YMODEM_OK)
        {
            Serial_SendString((uint8_t *)">> File uploaded successfully \n\r");
        }
        else
        {
            Serial_SendString((uint8_t *)">> Error Occurred while Transmitting File\n\r");
        }
    }
}

/**
 * @brief   Display the Main Menu on HyperTerminal
 *
 * @param   None
 *
 * @retval  None
 */
void Select_Menu(void)
{
    uint8_t key = 0xFFU;
    uint32_t address;

    Serial_SendString(
        (uint8_t *)"\r\n***************************************************************");
    Serial_SendString(
        (uint8_t *)"\r\n*      G32F031 In-Application Programming Application       *");
    Serial_SendString(
        (uint8_t *)"\r\n*                                              (Version 1.0.0)*");
    Serial_SendString(
        (uint8_t *)"\r\n***************************************************************");
    Serial_SendString((uint8_t *)"\r\n");

    /* Checks whether the FLASH Write Protection Status is set or not */
    if (IAP_OPT_ReadWriteProtect() != 0x0U)
    {
        Serial_SendString((uint8_t *)"Chip write protection unable to download program !***\r\n");
        Serial_SendString((uint8_t *)">> Jump to user application 1\r\n");

        address = USER_APP1_START_ADDRESS;

        /* Jump to user application */
        JumpAddress = *(__IO uint32_t *)(address + 4U);
        Jump_To_Application = (pFunction)JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t *)address);

        /* Jump to application */
        Jump_To_Application();
    }

    while (1)
    {
        Serial_SendString((uint8_t *)"\r\n** Please select an operation item \r\n");
        Serial_SendString(
            (uint8_t *)"*  1.Download Flash application 1 -----------------------> 1  *\r\n");
        Serial_SendString(
            (uint8_t *)"*  2.Upload Flash application 1   -----------------------> 2  *\r\n");
        Serial_SendString(
            (uint8_t *)"*  3.Jump to user application 1   -----------------------> 3  *\r\n");
        Serial_SendString(
            (uint8_t *)"*  4.Download Flash application 2 -----------------------> 4  *\r\n");
        Serial_SendString(
            (uint8_t *)"*  5.Upload Flash application 2   -----------------------> 5  *\r\n");
        Serial_SendString(
            (uint8_t *)"*  6.Jump to user application 2   -----------------------> 6  *\r\n");
        Serial_SendString(
            (uint8_t *)"***************************************************************\r\n");

        /* Receive key */
        while (DDL_USART_IsActiveFlag_RXNE(USART) == 0)
        {
        }
        key = DDL_USART_ReceiveData8(USART);

        switch (key)
        {
            /* Download user application in the Flash */
            case 0x31U:
                Download(IAP_APP1);
                break;

            /* Upload user application from the Flash */
            case 0x32U:
                Upload(IAP_APP1);
                break;

            /* Execute the new program */
            case 0x33U:
            case 0xFFU:
                Serial_SendString((uint8_t *)">> Jump to user application 1 \r\n");
                address = USER_APP1_START_ADDRESS;

                /* Jump to user application */
                JumpAddress = *(__IO uint32_t *)(address + 4U);
                Jump_To_Application = (pFunction)JumpAddress;

                /* Initialize user application's Stack Pointer */
                __set_MSP(*(__IO uint32_t *)address);

                /* Jump to application */
                Jump_To_Application();
                break;

            /* Execute the new program */
            case 0x34U:
                Download(IAP_APP2);
                break;

            /* Execute the new program */
            case 0x35U:
                Upload(IAP_APP2);
                break;

            /* Execute the new program */
            case 0x36U:
                Serial_SendString((uint8_t *)">> Jump to user application 2 \r\n");
                address = USER_APP2_START_ADDRESS;

                /* Jump to user application */
                JumpAddress = *(__IO uint32_t *)(address + 4U);
                Jump_To_Application = (pFunction)JumpAddress;

                /* Initialize user application's Stack Pointer */
                __set_MSP(*(__IO uint32_t *)address);

                /* Jump to application */
                Jump_To_Application();
                break;

            default:
                Serial_SendString((uint8_t *)">> Invalid Number ! ==> The number should be either "
                                             "1, 2 3 4 5 or 6\r\n");
                break;
        }
    }
}
