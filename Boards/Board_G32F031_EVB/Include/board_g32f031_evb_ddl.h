/**
 * @file        board_g32f031_evb_ddl.h
 *
 * @brief       This file contains definitions for G32F031_EVB's Leds, push-buttons hardware
 * resources
 *
 * @version     V1.0.1
 *
 * @date        2026-09-30
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
#ifndef BOARD_G32F031_EVB_DDL_H
#define BOARD_G32F031_EVB_DDL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ***************************************************************/
#include "g32f031_ddl_rcc.h"
#include "g32f031_ddl_bus.h"
#include "g32f031_ddl_eint.h"
#include "g32f031_ddl_gpio.h"

/* Exported macro *********************************************************/

/**
 * @brief LED1
 */
#define LED1_PIN                DDL_GPIO_PIN_6
#define LED1_GPIO_PORT          GPIOB
#define LED1_GPIO_CLK_ENABLE()  DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOB)
#define LED1_GPIO_CLK_DISABLE() DDL_AHB_GRP1_DisableClock(DDL_AHB_GRP1_PERIPH_GPIOB)

/**
 * @brief LED2
 */
#define LED2_PIN                DDL_GPIO_PIN_7
#define LED2_GPIO_PORT          GPIOB
#define LED2_GPIO_CLK_ENABLE()  DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOB)
#define LED2_GPIO_CLK_DISABLE() DDL_AHB_GRP1_DisableClock(DDL_AHB_GRP1_PERIPH_GPIOB)

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                DDL_GPIO_PIN_0
#define KEY1_BUTTON_GPIO_PORT          GPIOA
#define KEY1_BUTTON_GPIO_CLK_ENABLE()  DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA)
#define KEY1_BUTTON_GPIO_CLK_DISABLE() DDL_AHB_GRP1_DisableClock(DDL_AHB_GRP1_PERIPH_GPIOA)
#define KEY1_BUTTON_EINT_CLK_ENABLE()  DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_EINT)
#define KEY1_BUTTON_EINT_CLK_DISABLE() DDL_APB_GRP1_DisableClock(DDL_APB_GRP1_PERIPH_EINT)
#define KEY1_BUTTON_EINT_LINE          DDL_EINT_LINE_0
#define KEY1_BUTTON_EINT_PIN_PORT      DDL_EINT_SOURCE_PORTA
#define KEY1_BUTTON_EINT_IRQ_NUM       EINT0_1_IRQn

/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                DDL_GPIO_PIN_1
#define KEY2_BUTTON_GPIO_PORT          GPIOA
#define KEY2_BUTTON_GPIO_CLK_ENABLE()  DDL_AHB_GRP1_EnableClock(DDL_AHB_GRP1_PERIPH_GPIOA)
#define KEY2_BUTTON_GPIO_CLK_DISABLE() DDL_AHB_GRP1_DisableClock(DDL_AHB_GRP1_PERIPH_GPIOA)
#define KEY2_BUTTON_EINT_CLK_ENABLE()  DDL_APB_GRP1_EnableClock(DDL_APB_GRP1_PERIPH_EINT)
#define KEY2_BUTTON_EINT_CLK_DISABLE() DDL_APB_GRP1_DisableClock(DDL_APB_GRP1_PERIPH_EINT)
#define KEY2_BUTTON_EINT_LINE          DDL_EINT_LINE_1
#define KEY2_BUTTON_EINT_PIN_PORT      DDL_EINT_SOURCE_PORTA
#define KEY2_BUTTON_EINT_IRQ_NUM       EINT0_1_IRQn

/* Exported typedef *******************************************************/

/**
 * @brief  DAL Status structures definition
 */

/**
 * @brief LED type
 */
typedef enum {
    LED1 = 0U,
    LED2 = 1U,
} BOARD_LED_T;

/**
 * @brief Button type
 */
typedef enum {
    BUTTON_KEY1 = 0U,
    BUTTON_KEY2 = 1U,
} BOARD_BUTTON_T;

/**
 * @brief Button mode
 */
typedef enum {
    BUTTON_MODE_GPIO = 0U,
    BUTTON_MODE_EINT = 1U
} BOARD_BUTTON_MODE_T;

/* Exported function prototypes *******************************************/

/* Peripheral initialization functions */
uint32_t BOARD_LED_Config(BOARD_LED_T led);
uint32_t BOARD_BUTTON_Config(BOARD_BUTTON_T button, BOARD_BUTTON_MODE_T mode);

/* Peripheral control functions */
void BOARD_LED_On(BOARD_LED_T led);
void BOARD_LED_Off(BOARD_LED_T led);
void BOARD_LED_Toggle(BOARD_LED_T led);
uint32_t BOARD_BUTTON_GetState(BOARD_BUTTON_T button);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_G32F031_EVB_DDL_H */
