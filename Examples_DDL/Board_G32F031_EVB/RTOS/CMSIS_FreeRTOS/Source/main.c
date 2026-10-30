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

/* Private includes *******************************************************/
#include "cmsis_os2.h"
#include <stdio.h>
#include "board_g32f031_evb_ddl.h"

/* Private macro **********************************************************/

/* Private typedef ********************************************************/
static osThreadId_t ledThreadID = NULL;
static osThreadId_t usartThreadID = NULL;

static const osThreadAttr_t ledThreadattr = {
    .name = "LedToggle", .priority = osPriorityNormal, .stack_size = 512};

static const osThreadAttr_t usartThreadattr = {
    .name = "UsartTest", .priority = osPriorityNormal, .stack_size = 512};

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
/* Led toggle task */
void Led_Thread(void *argument);
/* Usart1 test task */
void Usart_TestThread(void *argument);

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
    /* Device configuration */
    Device_Config();

    /* Initialize the RTOS scheduler */
    osKernelInitialize();

    /* Create the thread */
    ledThreadID = osThreadNew(Led_Thread, NULL, &ledThreadattr);
    if (ledThreadID == NULL)
    {
        printf("Create led thread failed!\r\n");
    }

    usartThreadID = osThreadNew(Usart_TestThread, NULL, &usartThreadattr);
    if (usartThreadID == NULL)
    {
        printf("Create usart thread failed!\r\n");
    }

    /* Start the RTOS scheduler */
    osKernelStart();

    /* Infinite loop */
    while (1)
        ;
}

/**
 * @brief   Led toggle task
 *
 * @param   argument  Not used
 *
 * @retval  None
 */
void Led_Thread(void *argument)
{
    while (1)
    {
        /* Toggle LED2 */
        BOARD_LED_Toggle(LED2);
        osDelay(500);
    }
}

/**
 * @brief   Usart1 test task
 *
 * @param   argument  Not used
 *
 * @retval  None
 */
void Usart_TestThread(void *argument)
{
    while (1)
    {
        /* Print message */
        printf("Hello, world!\r\n");
        osDelay(1000);
    }
}
