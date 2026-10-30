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

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
volatile uint8_t seg1_flag = 0U;
volatile uint8_t seg2_flag = 0U;

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

    DDL_ADC_StartConversion(ADC);

    printf("ATMR Counter Enable\r\n");
    DDL_ATMR_EnableCounter(ATMR);

    /* Infinite loop */
    while (1)
    {
        if (seg1_flag == 1)
        {
            seg1_flag = 0;
            /* Read ADC conversion value */
            printf(
                "SEQ1 ADC CH0(PA7) Value: %ld mV\r\n",
                (unsigned long)(DDL_ADC_SEQ_ReadConversionData(ADC, DDL_ADC_REG_RANK_1) * 3300U) /
                    4095U);
        }
        if (seg2_flag == 1)
        {
            seg2_flag = 0;
            /* Read ADC conversion value */
            printf(
                "SEQ2 ADC CH1(PA8) Value: %ld mV\r\n",
                (unsigned long)(DDL_ADC_SEQ_ReadConversionData(ADC, DDL_ADC_REG_RANK_2) * 3300U) /
                    4095U);
            printf(
                "SEQ2 ADC CH0(PA7) Value: %ld mV\r\n",
                (unsigned long)(DDL_ADC_SEQ_ReadConversionData(ADC, DDL_ADC_REG_RANK_3) * 3300U) /
                    4095U);
        }
    }
}

void ADC_ISR(void)
{
    if (DDL_ADC_IsActiveFlag_EOSMP_SEQ_NUM1(ADC) && !seg1_flag)
    {
        DDL_ADC_ClearFlag_EOSMP_SEQ_NUM1(ADC);
        seg1_flag = 1;
    }
    if (DDL_ADC_IsActiveFlag_EOSMP_SEQ_NUM2(ADC) && !seg2_flag)
    {
        DDL_ADC_ClearFlag_EOSMP_SEQ_NUM2(ADC);
        seg2_flag = 1;
    }
}
