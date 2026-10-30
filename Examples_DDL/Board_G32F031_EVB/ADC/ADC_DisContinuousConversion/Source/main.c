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
volatile uint16_t adcConvValue[3][2] = {0U};
volatile uint16_t currentRanks = 0, nextRanks = 0;
volatile uint16_t currentTrigger = 0U, nextTrigger = 0U;
volatile uint16_t adcFinishFlg = 0U;

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

    /* Infinite loop */
    while (1)
    {
        if (nextTrigger >= 3U)
        {
            DDL_ADC_StopConversion(ADC);
            nextTrigger = 0U;
            adcFinishFlg = 1;
            printf("Trigger0, RANK1: ADC CH0 Value: %d mV\r\n",
                   ((adcConvValue[0][0] * 3300U) / 4095U));
            printf("Trigger0, RANK2: ADC CH1 Value: %d mV\r\n",
                   ((adcConvValue[0][1] * 3300U) / 4095U));
            printf("Trigger1, RANK3: ADC CH1 Value: %d mV\r\n",
                   ((adcConvValue[1][0] * 3300U) / 4095U));
            printf("Trigger1, RANK4: ADC CH0 Value: %d mV\r\n",
                   ((adcConvValue[1][1] * 3300U) / 4095U));
            printf("Trigger2, RANK5: ADC CH0 Value: %d mV\r\n",
                   ((adcConvValue[2][0] * 3300U) / 4095U));
            printf("Trigger2, RANK6: ADC CH1 Value: %d mV\r\n",
                   ((adcConvValue[2][1] * 3300U) / 4095U));
        }
        if (adcFinishFlg)
        {
            adcFinishFlg = 0;
            DDL_mDelay(1000);
            printf("\r\n");
            DDL_ADC_StartConversion(ADC);
        }
    }
}

void ADC_EOC_IntRequest(void)
{
    uint16_t trrgerIndex, ranksIndex;
    DDL_ADC_ClearFlag_EOC(ADC);
    /* Get the converted value */
    currentRanks = nextRanks;
    currentTrigger = nextTrigger;
    ranksIndex = currentRanks;
    trrgerIndex = currentTrigger;
    adcConvValue[trrgerIndex][ranksIndex] =
        DDL_ADC_REG_ReadConversionData(ADC); // Avoid IAR build warnings

    if (nextRanks >= 1)
    {
        nextRanks = 0;
        nextTrigger++;
        if (nextTrigger < 3)
        {
            DDL_ADC_StartConversion(ADC);
        }
    }
    else
    {
        nextRanks++;
    }
}

void ADC_EOS_IntRequest(void)
{
    DDL_ADC_ClearFlag_EOS(ADC);
    printf("ADC Discontinuous Conversion End \r\n");
}

void ADC_OVR_ERR_IntRequest(void)
{
    uint16_t trrgerIndex, ranksIndex;
    DDL_ADC_ClearFlag_OVR(ADC);
    DDL_ADC_StopConversion(ADC);
    ranksIndex = currentRanks;
    trrgerIndex = currentTrigger;
    printf("Trigger%d Conversion%d, ADC DATA OVER ERROR \r\n", trrgerIndex, ranksIndex);
    currentRanks = 0;
    nextRanks = 0;
}

void ADC_ISR(void)
{
    if (DDL_ADC_IsActiveFlag_OVR(ADC))
    {
        ADC_OVR_ERR_IntRequest();
    }

    if (DDL_ADC_IsActiveFlag_EOC(ADC))
    {
        ADC_EOC_IntRequest();
    }

    if (DDL_ADC_IsActiveFlag_EOS(ADC))
    {
        ADC_EOS_IntRequest();
    }
}
