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
volatile uint16_t adcConvValue[5] = {0U};
volatile uint16_t currentLoop = 0U, nextLoop = 0U;
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
    uint32_t i;

    /* Device configuration */
    Device_Config();

    DDL_ADC_StartConversion(ADC);

    /* Infinite loop */
    while (1)
    {
        if (nextLoop >= 5U)
        {
            DDL_ADC_StopConversion(ADC);
            nextLoop = 0U;
            adcFinishFlg = 1;
            for (i = 0U; i < 5U; i++)
            {
                printf("Loop%ld: ADC CH0 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i] * 3300U) / 4095U));
            }
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
    uint16_t loop;
    DDL_ADC_ClearFlag_EOC(ADC);
    /* Get the converted value */
    currentLoop = nextLoop;
    loop = currentLoop;
    adcConvValue[loop] = DDL_ADC_REG_ReadConversionData(ADC); // Avoid IAR build warnings
}

void ADC_EOS_IntRequest(void)
{
    DDL_ADC_ClearFlag_EOS(ADC);
    nextLoop++;
}

void ADC_OVR_ERR_IntRequest(void)
{
    uint16_t loop;
    DDL_ADC_ClearFlag_OVR(ADC);
    DDL_ADC_StopConversion(ADC);
    loop = currentLoop;
    printf("Loop%d, ADC DATA OVER ERROR \r\n", loop);
    nextLoop = 0;
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
