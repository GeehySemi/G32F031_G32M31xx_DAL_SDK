/**
 * @file        g32f031_dma_cfg.c
 *
 * @brief       This file provides all configuration support for DMA
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

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern uint32_t tmrDMABuffer[3];
/* External functions *****************************************************/

/**
 * @brief   DMA configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DMA_Config(void)
{
    DDL_DMA_InitTypeDef DMA_InitStruct = {0};

    DDL_RCC_REG_Unlock();
    DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_DMA);
    DDL_RCC_REG_Lock();

    /* Configure DMA parameters */
    DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&GTMR->CC0;
    DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&tmrDMABuffer[0];
    DMA_InitStruct.Direction = DDL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    DMA_InitStruct.Mode = DDL_DMA_MODE_CIRCULAR;
    DMA_InitStruct.PeriphOrM2MSrcIncMode = DDL_DMA_PERIPH_NOINCREMENT;
    DMA_InitStruct.MemoryOrM2MDstIncMode = DDL_DMA_MEMORY_INCREMENT;
    DMA_InitStruct.PeriphOrM2MSrcDataSize = DDL_DMA_PDATAALIGN_WORD;
    DMA_InitStruct.MemoryOrM2MDstDataSize = DDL_DMA_MDATAALIGN_WORD;
    DMA_InitStruct.NbData = 4;
    DMA_InitStruct.Priority = DDL_DMA_PRIORITY_HIGH;
    DMA_InitStruct.FIFOMode = DDL_DMA_FIFOMODE_DISABLE;
    DMA_InitStruct.FIFOThreshold = DDL_DMA_FIFOTHRESHOLD_FULL;
    DMA_InitStruct.MemBurst = DDL_DMA_MBURST_SINGLE;
    DMA_InitStruct.PeriphBurst = DDL_DMA_PBURST_SINGLE;
    DMA_InitStruct.Peripheral = DDL_DMA_PERIPHERAL_4;

    /* Initialize DMA */
    DDL_DMA_Init(DMA, DDL_DMA_CHANNEL_1, &DMA_InitStruct);

    DDL_DMA_EnableChannel(DMA, DDL_DMA_CHANNEL_1);
}
