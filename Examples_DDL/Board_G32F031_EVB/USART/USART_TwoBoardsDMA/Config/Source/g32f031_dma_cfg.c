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
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern char rxBuffer[50U];
extern char txBufferStr[];
extern uint32_t txBuffer[];
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

    DDL_DMA_DisableChannel(DMA, DDL_DMA_CHANNEL_0);
    DDL_DMA_DisableChannel(DMA, DDL_DMA_CHANNEL_1);

    /* Configure DMA parameters */
    /* USART RX */
    DMA_InitStruct.PeriphOrM2MSrcAddress = DDL_USART_DMA_GetRegAddr(USART);
    DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)rxBuffer;
    DMA_InitStruct.Direction = DDL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    DMA_InitStruct.Mode = DDL_DMA_MODE_NORMAL;
    DMA_InitStruct.PeriphOrM2MSrcIncMode = DDL_DMA_PERIPH_NOINCREMENT;
    DMA_InitStruct.MemoryOrM2MDstIncMode = DDL_DMA_MEMORY_INCREMENT;
    DMA_InitStruct.PeriphOrM2MSrcDataSize = DDL_DMA_PDATAALIGN_BYTE;
    DMA_InitStruct.MemoryOrM2MDstDataSize = DDL_DMA_MDATAALIGN_BYTE;
    DMA_InitStruct.NbData = (uint32_t)strlen(txBufferStr);
    DMA_InitStruct.Priority = DDL_DMA_PRIORITY_HIGH;
    DMA_InitStruct.FIFOMode = DDL_DMA_FIFOMODE_DISABLE;
    DMA_InitStruct.FIFOThreshold = DDL_DMA_FIFOTHRESHOLD_FULL;
    DMA_InitStruct.MemBurst = DDL_DMA_MBURST_SINGLE;
    DMA_InitStruct.PeriphBurst = DDL_DMA_PBURST_SINGLE;
    DMA_InitStruct.Peripheral = DDL_DMA_PERIPHERAL_11;
    DDL_DMA_Init(DMA, DDL_DMA_CHANNEL_1, &DMA_InitStruct);

    DDL_DMA_EnableIT_TC(DMA, DDL_DMA_CHANNEL_1);
    DDL_DMA_ClearFlag_TC1(DMA);
    NVIC_SetPriority(DMA_CH1_IRQn, 0U);
    NVIC_EnableIRQ(DMA_CH1_IRQn);

    /* USART TX */
    DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)txBuffer;
    DMA_InitStruct.Direction = DDL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    DMA_InitStruct.PeriphOrM2MSrcDataSize = DDL_DMA_PDATAALIGN_WORD;
    DMA_InitStruct.MemoryOrM2MDstDataSize = DDL_DMA_MDATAALIGN_WORD;
    DMA_InitStruct.Priority = DDL_DMA_PRIORITY_LOW;
    DDL_DMA_Init(DMA, DDL_DMA_CHANNEL_0, &DMA_InitStruct);

    DDL_DMA_EnableIT_TC(DMA, DDL_DMA_CHANNEL_0);
    DDL_DMA_ClearFlag_TC0(DMA);
    NVIC_SetPriority(DMA_CH0_IRQn, 0U);
    NVIC_EnableIRQ(DMA_CH0_IRQn);
}
