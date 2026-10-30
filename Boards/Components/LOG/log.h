/**
 * @file        log.h
 * @brief       Header file containing functions prototypes of LOG
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>

/* Exported macro ------------------------------------------------------------*/

#ifndef USE_LOG_COMPONENT
#define USE_LOG_COMPONENT 1U
#endif /* USE_LOG_COMPONENT */

#ifndef USE_LOG_COLOR
#define USE_LOG_COLOR 0U
#endif /* USE_LOG_COLOR */

#define LOG_END_CHAR "\r\n"

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR_WHITE   "37"
#define LOG_COLOR_DEFAULT "39"

#define LOG_BG_COLOR_BLACK  "40"
#define LOG_BG_COLOR_RED    "41"
#define LOG_BG_COLOR_GREEN  "42"
#define LOG_BG_COLOR_BROWN  "43"
#define LOG_BG_COLOR_BLUE   "44"
#define LOG_BG_COLOR_PURPLE "45"
#define LOG_BG_COLOR_CYAN   "46"
#define LOG_BG_COLOR_WHITE  "47"

#define LOG_COLOR(COLOR)        "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)         "\033[1;" COLOR "m"
#define LOG_BG_BLK_COLOR(COLOR) "\033[40;" COLOR "m"
#define LOG_COLOR_RESET         "\033[0m"

#if USE_LOG_COLOR == 1U
#define LOG_COLOR_DEBUG   LOG_COLOR(LOG_COLOR_GREEN) "[%s] " LOG_COLOR(LOG_COLOR_DEFAULT)
#define LOG_COLOR_INFO    LOG_COLOR(LOG_COLOR_BLUE) "[%s] " LOG_COLOR(LOG_COLOR_DEFAULT)
#define LOG_COLOR_WARNING LOG_COLOR(LOG_COLOR_BROWN) "[%s] " LOG_COLOR(LOG_COLOR_DEFAULT)
#define LOG_COLOR_ERROR   LOG_COLOR(LOG_COLOR_RED) "[%s] " LOG_COLOR(LOG_COLOR_DEFAULT)
#else
#define LOG_COLOR_DEBUG   "[%s] "
#define LOG_COLOR_INFO    "[%s] "
#define LOG_COLOR_WARNING "[%s] "
#define LOG_COLOR_ERROR   "[%s] "
#endif /* USE_LOG_COLOR == 1U */

#define LOG_FORMAT(text, tag, format, ...) LOG_Print(text format LOG_END_CHAR, tag, ##__VA_ARGS__)

/* Exported types ------------------------------------------------------------*/

/**
 * @brief log level
 */
typedef enum {
    LOG_NONE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
} LOG_LEVEL_T;
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/

typedef int (*LOG_FUNC_T)(const char *, va_list);

/* Control functions */
void LOG_SetCallback(LOG_FUNC_T callback);
void LOG_Print(const char *format, ...);

#if (USE_LOG_COMPONENT == 1U)
#define LOGI(tag, format, ...) LOG_FORMAT(LOG_COLOR_INFO, tag, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) LOG_FORMAT(LOG_COLOR_ERROR, tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) LOG_FORMAT(LOG_COLOR_WARNING, tag, format, ##__VA_ARGS__)
#define LOGD(tag, format, ...) LOG_FORMAT(LOG_COLOR_DEBUG, tag, format, ##__VA_ARGS__)
#else
#define LOGI(tag, format, ...) ((void)0U)
#define LOGE(tag, format, ...) ((void)0U)
#define LOGW(tag, format, ...) ((void)0U)
#define LOGD(tag, format, ...) ((void)0U)
#endif /* USE_LOG_COMPONENT == 1U */

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
