/**
 * @file        common.c
 *
 * @brief       Include data convert and read and write operation to HyperTerminal
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
#include "iap_common.h"

/* Private includes *******************************************************/
#include "g32f031_device_cfg.h"

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Integer number convert to string
 *
 * @param   str: string buffer
 *
 * @param   conint: specific interger number
 *
 * @retval  None
 */
void IntConStr(uint8_t *str, uint32_t conint)
{
    uint32_t i;
    uint32_t j;
    uint32_t div;
    uint32_t convStatus;

    j = 0;
    i = 0;
    div = 1000000000;
    convStatus = 0;

    do
    {
        str[j++] = (conint / div) + 48;

        conint = conint % div;
        div /= 10;

        if ((str[j - 1] == '0') & (convStatus == 0))
        {
            j = 0;
        }
        else
        {
            convStatus++;
        }

        i++;
    } while (i < 10);

    if (j == 0U)
    {
        str[j++] = '0';
    }

    str[j] = '\0';
}

/**
 * @brief   string convert to an integer number
 *
 * @param   hexstr: hex string type data
 *
 * @param   conint: specific interger number
 *
 * @retval  SUCCESS
 *          ERROR
 */
uint32_t Str_Hex_ConInt(uint8_t *hexstr, uint32_t *conint)
{
    uint32_t i = 0;
    uint32_t res = ERROR;
    uint32_t val = 0;

    if ((hexstr[2] != '\0'))
    {
        i = 2;

        do
        {
            if (hexstr[i] == '\0')
            {
                *conint = val;
                res = SUCCESS;
                break;
            }

            if (CHECK_HEX(hexstr[i]))
            {
                val = (val << 4) + CON_HEX(hexstr[i]);
            }
            else
            {
                res = ERROR;
                break;
            }

            i++;
        } while (i < 11);

        /* over 8 digit hex */
        if (i >= 11)
        {
            res = ERROR;
        }
    }
    else
    {
        return ERROR;
    }

    return res;
}

/**
 * @brief   decimal string convert to an integer
 *
 * @param   decstr: dec string buffer
 *
 * @param   conint: specific interger number
 *
 * @retval  SUCCESS
 *          ERROR
 */
uint32_t Str_Dec_ConInt(uint8_t *decstr, uint32_t *conint)
{
    uint32_t i = 0;
    uint32_t res = ERROR;
    uint32_t val = 0;

    i = 0;
    do
    {
        if (decstr[i] == '\0')
        {
            *conint = val;
            res = SUCCESS;
            break;
        }
        else if ((decstr[i] == 'k' || decstr[i] == 'K') && (i > 0))
        {
            val = val << 10;
            *conint = val;
            res = SUCCESS;
            break;
        }
        else if ((decstr[i] == 'm' || decstr[i] == 'M') && (i > 0))
        {
            val = val << 20;
            *conint = val;
            res = SUCCESS;
            break;
        }
        else if (CHECK_DEC(decstr[i]))
        {
            val = val * 10 + CON_DEC(decstr[i]);
        }
        else
        {
            res = ERROR;
            break;
        }

        i++;
    } while (i < 11);

    /* Over 10 digit decimal */
    if (i >= 11)
    {
        res = ERROR;
    }

    return res;
}

/**
 * @brief   string type convert to integer type
 *
 * @param   str: string buffer
 *
 * @param   conint: specific interger number
 *
 * @retval  SUCCESS
 *          ERROR
 */
uint32_t StrConInt(uint8_t *str, uint32_t *conint)
{
    uint32_t res = ERROR;

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        res = Str_Hex_ConInt(str, conint);
    }
    else
    {
        res = Str_Dec_ConInt(str, conint);
    }

    return res;
}

/**
 * @brief   Send a string on the HyperTerminal
 *
 * @param   str: The string need to be sent
 *
 * @retval  None
 */
void Serial_SendString(uint8_t *str)
{
    uint16_t length = 0;

    while (str[length] != '\0')
    {
        DDL_USART_TransmitData8(USART, str[length]);
        while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
            ;
        length++;
    }
}

/**
 * @brief   Send a byte on the HyperTerminal
 *
 * @param   data: The byte need to be sent
 *
 * @retval  DDL status
 */
ErrorStatus Serial_SendByte(uint8_t data)
{
    DDL_USART_TransmitData8(USART, data);
    while (DDL_USART_IsActiveFlag_TXE(USART) == 0)
    {
    }

    return SUCCESS;
}

/**
 * @brief   Receive a byte from the HyperTerminal
 *
 * @param   data: The byte need to be received
 *
 * @param   revTimeout:
 *
 * @retval  DDL status
 */
ErrorStatus Receive_Byte(uint8_t *data, uint32_t revTimeout)
{
    while (revTimeout-- > 0)
    {
        if (DDL_USART_IsActiveFlag_RXNE(USART))
        {
            *data = DDL_USART_ReceiveData8(USART);
            return SUCCESS;
        }
    }
    return ERROR;
}
