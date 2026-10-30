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
volatile uint16_t adcConvValue[5][5] = {{0U}};
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

    DDL_DMA_EnableChannel(DMA, DDL_DMA_CHANNEL_1);

    DDL_ADC_StartConversion(ADC);

    /* Infinite loop */
    while (1)
    {
        if (adcFinishFlg)
        {
            adcFinishFlg = 0;
            for (i = 0U; i < 5U; i++)
            {
                printf("Loop%ld, RANK1: ADC CH0 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i][0] * 3300U) / 4095U));
                printf("Loop%ld, RANK2: ADC CH1 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i][1] * 3300U) / 4095U));
                printf("Loop%ld, RANK3: ADC CH1 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i][2] * 3300U) / 4095U));
                printf("Loop%ld, RANK4: ADC CH0 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i][3] * 3300U) / 4095U));
                printf("Loop%ld, RANK5: ADC CH1 Value: %d mV\r\n",
                       (unsigned long)i,
                       ((adcConvValue[i][4] * 3300U) / 4095U));
            }
            DDL_mDelay(1000);
            printf("\r\n");
            DDL_ADC_StartConversion(ADC);
        }
    }
}

void ADC_OVR_ERR_IntRequest(void)
{
    DDL_ADC_ClearFlag_OVR(ADC);
    DDL_ADC_StopConversion(ADC);
    printf("ADC DATA OVER ERROR \r\n");
}

void DDL_DMA_IRQHandler(void)
{
    if (DDL_DMA_IsActiveFlag_TC1(DMA))
    {
        DDL_DMA_ClearFlag_TC1(DMA);
        DDL_ADC_StopConversion(ADC);
        adcFinishFlg = 1;
    }
    if (DDL_DMA_IsActiveFlag_TE1(DMA))
    {
        DDL_DMA_ClearFlag_TE1(DMA);
        printf("DMA Channel 1 transfer error \r\n");
    }
    if (DDL_DMA_IsActiveFlag_DME1(DMA))
    {
        DDL_DMA_ClearFlag_DME1(DMA);
        printf("DMA Channel 1 direct mode error \r\n");
    }
}
void ADC_ISR(void)
{
    if (DDL_ADC_IsActiveFlag_OVR(ADC))
    {
        ADC_OVR_ERR_IntRequest();
    }
}
