/**
  *
  * @file        g32_ddl_legacy.h
  *
  * @brief       This file contains aliases definition for the DDL constants
  *              macros and functions maintained for legacy purpose.
  *
  * @version     V1.0.1
  *
  * @date        2026-09-30
  *
  * @attention
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *
  * 1. Redistributions of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of the copyright holder nor the names of its contributors
  *    may be used to endorse or promote products derived from this software without
  *    specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  * OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * The original code has been modified by Geehy Semiconductor.
  *
 *  Copyright (C) 2026 Geehy Semiconductor
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef G32_DDL_LEGACY_H
#define G32_DDL_LEGACY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup DDL_ATMR_Aliased_Defines ATMR Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_ATMR_OC_ConfigOutput              DDL_ATMR_OC_Config
/**
  * @}
  */

/** @defgroup DDL_BTMR_Aliased_Defines BTMR Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_BTMR_OC_ConfigOutput              DDL_BTMR_OC_Config
/**
  * @}
  */

/** @defgroup DDL_COMP0_Aliased_Defines COMP0 Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_COMP0_EnableSWINT                 DDL_COMP0_GenerateSWINT
#define DDL_COMP0_INPUT_MINUS_PGA0            DDL_COMP0_INPUT_MINUS_OPAMP0
#define DDL_COMP0_INPUT_MINUS_PGA1            DDL_COMP0_INPUT_MINUS_OPAMP1
#define DDL_COMP0_HYSP_60MV                   DDL_COMP0_HYSP_80MV
#define DDL_COMP0_HYSN_60MV                   DDL_COMP0_HYSN_80MV
/**
  * @}
  */

/** @defgroup DDL_COMP1_Aliased_Defines COMP1 Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_COMP1_EnableSWINT                 DDL_COMP1_GenerateSWINT
#define DDL_COMP1_HYSP_60MV                   DDL_COMP1_HYSP_80MV
#define DDL_COMP1_HYSN_60MV                   DDL_COMP1_HYSN_80MV
/**
  * @}
  */

/** @defgroup DDL_DIV_Aliased_Defines DIV Aliased Defines maintained for legacy purpose
  * @{
  */
  #define DDL_DIV_DisalbeUnsignedDiv          DDL_DIV_DisableUnsignedDiv
  /**
    * @}
    */

/** @defgroup DDL_FLASH_Aliased_Defines FLASH Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_FLASH_RKEY_Unlock                 DDL_FLASH_REG_Unlock
#define DDL_FLASH_RKEY_Lock                   DDL_FLASH_REG_Lock
#define DDL_FLASH_RKEY_IsLocked               DDL_FLASH_REG_IsLocked
#define DDL_FLASH_MKEY_Unlock                 DDL_FLASH_MAIN_Unlock
#define DDL_FLASH_MKEY_Lock                   DDL_FLASH_MAIN_Lock
#define DDL_FLASH_MKEY_IsLocked               DDL_FLASH_MAIN_IsLocked
#define DDL_FLASH_NVRCKEY_Unlock              DDL_FLASH_NVRC_Unlock
#define DDL_FLASH_NVRCKEY_Lock                DDL_FLASH_NVRC_Lock
#define DDL_FLASH_NVRCKEY_IsLocked            DDL_FLASH_NVRC_IsLocked
#define DDL_Flash_EnableReadOnly              DDL_FLASH_EnableReadOnly
#define DDL_Flash_DisableReadOnly             DDL_FLASH_DisableReadOnly
#define DDL_Flash_EnablePreFetch              DDL_FLASH_EnablePreFetch
#define DDL_Flash_DisablePreFetch             DDL_FLASH_DisablePreFetch
#define DDL_Flash_IsEnabled_PreFetch          DDL_FLASH_IsEnabled_PreFetch
#define DDL_Flash_EnableForceOptload          DDL_FLASH_EnableForceOptload
#define DDL_FLASH_IsActiveFlag_KEY            DDL_FLASH_IsActiveFlag_KEYERR
#define DDL_FLASH_ClearFlag_KEY               DDL_FLASH_ClearFlag_KEYERR
#define DDL_FLASH_IsActiveFlag_RPT            DDL_FLASH_IsActiveFlag_RPTERR
#define DDL_FLASH_ClearFlag_RPT               DDL_FLASH_ClearFlag_RPTERR
/**
  * @}
  */

/** @defgroup DDL_GPIO_Aliased_Defines GPIO Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_GPIO_SetAF3Pin                    DDL_GPIO_SetAF3Pin_0_15
#define DDL_GPIO_GetAF3Pin                    DDL_GPIO_GetAF3Pin_0_15
/**
  * @}
  */

/** @defgroup DDL_PMU_Aliased_Defines PMU Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_PMU_Unlock                        DDL_PMU_REG_Unlock
#define DDL_PMU_Lock                          DDL_PMU_REG_Lock
#define DDL_PMU_IsActiveFlag_LOCK             DDL_PMU_REG_IsLocked
#define DDL_PMU_EnableWakeUpStandbyPin0       DDL_PMU_EnableWakeUpPin0
#define DDL_PMU_DisableWakeUpStandbyPin0      DDL_PMU_DisableWakeUpPin0
#define DDL_PMU_EnableWakeUpStandbyPin1       DDL_PMU_EnableWakeUpPin1
#define DDL_PMU_DisableWakeUpStandbyPin1      DDL_PMU_DisableWakeUpPin1
#define DDL_PMU_SetWakeUpStandbyPin0Polarity  DDL_PMU_SetWakeUpPolarityPin0
#define DDL_PMU_GetWakeUpStandbyPin0Polarity  DDL_PMU_GetWakeUpPolarityPin0
#define DDL_PMU_SetWakeUpStandbyPin1Polarity  DDL_PMU_SetWakeUpPolarityPin1
#define DDL_PMU_GetWakeUpStandbyPin1Polarity  DDL_PMU_GetWakeUpPolarityPin1
#define DDL_PMU_SetWakeUpStandbyPin0Pull      DDL_PMU_SetWakeUpPullPin0
#define DDL_PMU_GetWakeUpStandbyPin0Pull      DDL_PMU_GetWakeUpPullPin0
#define DDL_PMU_SetWakeUpStandbyPin1Pull      DDL_PMU_SetWakeUpPullPin1
#define DDL_PMU_GetWakeUpStandbyPin1Pull      DDL_PMU_GetWakeUpPullPin1
#define DDL_PMU_EnableIWDTWakeUpStandby       DDL_PMU_EnableWakeUpIWDT
#define DDL_PMU_DisableIWDTWakeUpStandby      DDL_PMU_DisableWakeUpIWDT
#define DDL_PMU_EnableLPTMRWakeUpStandby      DDL_PMU_EnableWakeUpLPTMR
#define DDL_PMU_DisableLPTMRWakeUpStandby     DDL_PMU_DisableWakeUpLPTMR
/**
  * @}
  */

/** @defgroup DDL_RCC_Aliased_Defines RCC Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_RCC_Unlock                        DDL_RCC_REG_Unlock
#define DDL_RCC_Lock                          DDL_RCC_REG_Lock
#define DDL_RCC_IsActiveFlag_LOCK             DDL_RCC_REG_IsLocked
/**
  * @}
  */

/** @defgroup DDL_SCU_Aliased_Defines SCU Aliased Defines maintained for legacy purpose
  * @{
  */
#define DDL_SCU_Unlock                        DDL_SCU_REG_Unlock
#define DDL_SCU_Lock                          DDL_SCU_REG_Lock
#define DDL_SCU_IsActiveFlag_LOCK             DDL_SCU_REG_IsLocked
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* G32_DDL_LEGACY_H */
