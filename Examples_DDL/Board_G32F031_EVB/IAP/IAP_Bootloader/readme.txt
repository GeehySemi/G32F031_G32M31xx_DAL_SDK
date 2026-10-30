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

The example aims to show how to configure a bootloader firmware to IAP.
When device connect to HyperTerminal right, a usart menu will show to user.
The menu as follow:

***************************************************************
*      G32F031 Application Programming Application      *
*                                              (Version 1.0.0)*
**************************************************************
** Please select an operation item
*  1.Download Flash application 1 -----------------------> 1  *
*  2.Upload Flash application 1   -----------------------> 2  *
*  3.Jump to user application 1   -----------------------> 3  *
*  4.Download Flash application 2 -----------------------> 4  *
*  5.Upload Flash application 2   -----------------------> 5  *
*  6.Jump to user application 2   -----------------------> 6  *
***************************************************************

Flash ROM Space:
 - IAP_Bootloader       0x08000000 - 0x08003FFF
 - IAP_Application1     0x08004000 - 0x08005FFF
 - IAP_Application2     0x08006000 - 0x08007FFF

note:

Before using IAP function, you must download the bootloader program to the target board.
Then compile the "IAP_Application1" or "IAP_Application2" project, and update the generated bin
firmware file to the target board with the function of IAP through the HyperTerminal tool.

&par Directory contents

  - IAP/IAP_Bootloader/Source/main.c               Main program
  - IAP/IAP_Bootloader/Source/g32f031_int.c        Interrupt handlers

&par Hardware Description

The USART is configured as follows:
    - TX:PA10, RX:PA11
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
