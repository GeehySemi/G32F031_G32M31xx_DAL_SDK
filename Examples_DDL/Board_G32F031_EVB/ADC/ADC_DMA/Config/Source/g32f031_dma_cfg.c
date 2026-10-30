/**
 * @file        g32f031_dma_cfg.c
 *
 * @brief       This file provides configuration support for DMA
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
extern volatile uint16_t adcConvValue[5][5];
/* External functions *****************************************************/

/**
 * @brief   ADC configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DMA_Config(void)
{
    DDL_DMA_InitTypeDef DMA_InitStruct = {0U};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_DMA);
    DDL_RCC_REG_Lock();

    DMA_InitStruct.Peripheral = DDL_DMA_PERIPHERAL_0;
    DMA_InitStruct.Direction = DDL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&(ADC->DR);
    DMA_InitStruct.PeriphOrM2MSrcDataSize = DDL_DMA_PDATAALIGN_HALFWORD;
    DMA_InitStruct.PeriphOrM2MSrcIncMode = DDL_DMA_PERIPH_NOINCREMENT;
    DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)adcConvValue;
    DMA_InitStruct.MemoryOrM2MDstDataSize = DDL_DMA_MDATAALIGN_HALFWORD;
    DMA_InitStruct.MemoryOrM2MDstIncMode = DDL_DMA_MEMORY_INCREMENT;
    DMA_InitStruct.NbData = 25;
    DMA_InitStruct.Mode = DDL_DMA_MODE_CIRCULAR;
    DMA_InitStruct.Priority = DDL_DMA_PRIORITY_LOW;
    DMA_InitStruct.FIFOMode = DDL_DMA_FIFOMODE_DISABLE;

    DDL_DMA_DisableChannel(DMA, DDL_DMA_CHANNEL_1);
    while (DDL_DMA_IsEnabledChannel(DMA, DDL_DMA_CHANNEL_1) != 0)
    {
    }

    DDL_DMA_Init(DMA, DDL_DMA_CHANNEL_1, &DMA_InitStruct);
    DMA->IFCLR = 0xFFFFFFFF;
    DDL_DMA_EnableIT_TC(DMA, DDL_DMA_CHANNEL_1);
    DDL_DMA_EnableIT_TE(DMA, DDL_DMA_CHANNEL_1);
    DDL_DMA_EnableIT_DME(DMA, DDL_DMA_CHANNEL_1);

    NVIC_SetPriority((DMA_CH1_IRQn), 1u);
    NVIC_EnableIRQ((DMA_CH1_IRQn));
}
