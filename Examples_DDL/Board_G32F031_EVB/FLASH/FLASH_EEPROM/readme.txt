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

The example describes a software method, using the onchip Flash memory of the
G32F031 devices to emulate eeprom. After running this example, if the reading
and writing successful, LED2 is on, otherwise it is off.
The print data will have displayed on serial assistant through USART.

The following is the G32F031 Flash region planning for this example.
/*
 +--------------------------------------------------------------+
 |                        Flash                                 |
 +--------+-----------------+-----------------------------------+
 |  Code  |   Flash EEPROM  |                Code               |
 +--------+--------+--------+--------+--------+-----------------+
 |  16KB  |  512B  |  512B  |  512B  |  512B  |  512B  |  512B  |
 | sector | sector | sector | sector | sector | sector | sector |
 |   20   |   21   |   22   |   23   |   24   |   N    | N + 1  |
 |        |        |        |        |        |        |        |
 +--------+--------+--------+--------+--------+-----------------+
*/

The USART is configured as follows:
    - TX:PA10, RX:PA11
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - FLASH/FLASH_EEPROM/Source/main.c               Main program
  - FLASH/FLASH_EEPROM/Source/g32f031_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
