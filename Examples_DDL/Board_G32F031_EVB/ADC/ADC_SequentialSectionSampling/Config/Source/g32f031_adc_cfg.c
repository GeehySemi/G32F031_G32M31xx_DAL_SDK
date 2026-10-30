/**
 * @file        g32f031_adc_cfg.c
 *
 * @brief       This file provides configuration support for ADC
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
#include "g32f031_device_cfg.h"

/* Private includes *******************************************************/
#include <stdio.h>

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   ADC configuration
 *
 * @param   None
 *
 * @retval  None
 */
void ADC_Config(void)
{
    DDL_GPIO_InitTypeDef GPIO_InitStruct = {0U};
    DDL_ADC_InitTypeDef ADC_InitStruct = {0U};
    DDL_ADC_SEQ_InitTypeDef ADC_SEQ_InitStruct = {0U};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA);
    DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_ADC);
    DDL_RCC_REG_Lock();

    /* Configure the ADC pin */
    GPIO_InitStruct.Pin = DDL_GPIO_PIN_7 | DDL_GPIO_PIN_8; // ADC_IN0, ADC_IN1
    GPIO_InitStruct.Mode = DDL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Drive = DDL_GPIO_DRIVE_LOW;
    GPIO_InitStruct.OutputType = DDL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.InputEnable = DDL_GPIO_INPUT_ENABLE;
    GPIO_InitStruct.Pull = DDL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = DDL_GPIO_AF_0;
    DDL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DDL_GPIO_REG_Lock(GPIOA);

    /* Set ADC Prescaler value */
    DDL_RCC_REG_Unlock();
    DDL_RCC_SetADCClkDiv(DDL_RCC_ADCCLK_DIVISION_4);
    DDL_RCC_REG_Lock();

    DDL_ADC_Enable(ADC);
    while (!DDL_ADC_IsActiveFlag_RDY(ADC)){}

    DDL_ADC_ClearFlag_EOC(ADC);
    DDL_ADC_ClearFlag_OVR(ADC);

    ADC_InitStruct.DataAlignment = DDL_ADC_ALIGNMENT_RIGHT;
    DDL_ADC_Init(ADC, &ADC_InitStruct);

    ADC_SEQ_InitStruct.NumSec = DDL_ADC_SEQ_NUMBER_2;
    ADC_SEQ_InitStruct.TimesSec1 = DDL_ADC_SEQ_TRS_TIME_1;
    ADC_SEQ_InitStruct.TimesSec2 = DDL_ADC_SEQ_TRS_TIME_2;
    ADC_SEQ_InitStruct.TimesSec3 = DDL_ADC_SEQ_TRS_TIME_1;
    ADC_SEQ_InitStruct.GapTime = DDL_ADC_GAPTIME_ADCCLK_0;
    ADC_SEQ_InitStruct.TriggerSourceSec1 = DDL_ADC_SEQ_TRIG_EXTSEL_ATMR_TRGO1;
    ADC_SEQ_InitStruct.TriggerSourceSec2 = DDL_ADC_SEQ_TRIG_EXTSEL_ATMR_TRGO0;
    ADC_SEQ_InitStruct.TriggerSourceSec3 = DDL_ADC_SEQ_TRIG_EXTSEL_ATMR_TRGO0;
    ADC_SEQ_InitStruct.TriggerEdgeSec1 = DDL_ADC_SEQ_TRIG_EXTEDGE_RISING;
    ADC_SEQ_InitStruct.TriggerEdgeSec2 = DDL_ADC_SEQ_TRIG_EXTEDGE_RISING;
    ADC_SEQ_InitStruct.TriggerEdgeSec3 = DDL_ADC_SEQ_TRIG_EXTEDGE_RISING;
    DDL_ADC_SEQ_Init(ADC, &ADC_SEQ_InitStruct);

    /* Configure for the selected ADC regular channel */
    DDL_ADC_REG_SetSequencerRanks(ADC, DDL_ADC_REG_RANK_1, DDL_ADC_CHANNEL_0);
    DDL_ADC_REG_SetSequencerRanks(ADC, DDL_ADC_REG_RANK_2, DDL_ADC_CHANNEL_1);
    DDL_ADC_REG_SetSequencerRanks(ADC, DDL_ADC_REG_RANK_3, DDL_ADC_CHANNEL_0);

    /* Set sampling time  */
    DDL_ADC_SetChannelSamplingTime(ADC, DDL_ADC_CHANNEL_0, DDL_ADC_SAMPLINGTIME_256_SCYCLES);
    DDL_ADC_SetChannelSamplingTime(ADC, DDL_ADC_CHANNEL_1, DDL_ADC_SAMPLINGTIME_256_SCYCLES);

    DDL_ADC_SEQ_Enable(ADC);
    DDL_ADC_EnableIT_EOSMP_SEQ_NUM1(ADC);
    DDL_ADC_EnableIT_EOSMP_SEQ_NUM2(ADC);

    /* Enable and set ADC Interrupt priority */
    NVIC_SetPriority((ADC_IRQn), 0U);
    NVIC_EnableIRQ((ADC_IRQn));
}
