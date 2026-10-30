/**
 * @file        g32f031_device_cfg.h
 *
 * @brief       This file provides all configuration support for device
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

/* Define to prevent recursive inclusion */
#ifndef G32F031_DEVICE_CFG_H
#define G32F031_DEVICE_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "g32f031_ddl_adc.h"
#include "g32f031_ddl_atmr.h"
#include "g32f031_ddl_btmr.h"
#include "g32f031_ddl_bus.h"
#include "g32f031_ddl_comp0.h"
#include "g32f031_ddl_comp1.h"
#include "g32f031_ddl_cortex.h"
#include "g32f031_ddl_crc.h"
#include "g32f031_ddl_div.h"
#include "g32f031_ddl_dma.h"
#include "g32f031_ddl_eint.h"
#include "g32f031_ddl_flash.h"
#include "g32f031_ddl_gpio.h"
#include "g32f031_ddl_gtmr.h"
#include "g32f031_ddl_i2c.h"
#include "g32f031_ddl_iwdt.h"
#include "g32f031_ddl_lptmr.h"
#include "g32f031_ddl_opa.h"
#include "g32f031_ddl_pmu.h"
#include "g32f031_ddl_rcc.h"
#include "g32f031_ddl_scu.h"
#include "g32f031_ddl_spi.h"
#include "g32f031_ddl_usart.h"
#include "g32f031_ddl_utils.h"
#include "g32f031_ddl_wwdt.h"

#include "g32f031_usart_cfg.h"

#include "board_g32f031_evb_ddl.h"
#if defined(USE_FULL_ASSERT)
#include "g32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported macro *********************************************************/

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
void Device_Config(void);
void Device_Reset(void);
void SysClk_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* G32F031_DEVICE_CFG_H */
