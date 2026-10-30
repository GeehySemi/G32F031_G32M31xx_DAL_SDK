/**
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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

&par Example Description

This example shows how to use UART to communicate between 1st board and 2nd board by interrupt.
1st board uses UART to send a string and then enters the data waiting for 2nd board to return data,
while 2nd board uses UART to receive the string of 1st board, and then sends back the data.
If the data received and sent are inconsistent, the LED2 of 1st board and 2nd board will turn on,
otherwise LED2 keep flashing.

note:The two boards share a firmware project.

&par Hardware Description

  - UART configured as follow:
  - BaudRate    = 115200
  - Word Length = UART_WordLength_8b
  - Stop Bit    = UART_StopBits_1
  - Parity      = UART_Parity_No
  - Transmit and Receive enabled

  - use PB1 as UART TX
  - use PB2 as UART RX

&par Directory contents

  - UART/UART_TwoBoardsInterrupt/Source/main.c               Main program
  - UART/UART_TwoBoardsInterrupt/Source/g32f031_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 Tiny Devices.
