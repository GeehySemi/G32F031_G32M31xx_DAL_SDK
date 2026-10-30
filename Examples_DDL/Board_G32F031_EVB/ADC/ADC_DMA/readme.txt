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

This example provides a example of how to use a DMA channel to transfer
continuously a data from a peripheral(ADC_DR) to memory(array).
And use ADC to monitor the voltage of ADC_CH0 and ADC_CH1 continuously, and
ADC trigger source set with software.

Regular channel sequence length set with 5, and loop 5 times, then stop the ADC conversion.
Rank1(ADC_CH0)
Rank2(ADC_CH1)
Rank3(ADC_CH1)
Rank4(ADC_CH0)
Rank5(ADC_CH1)
The converted voltage is displayed on serial assistant through USART.

&par Hardware Description

The USART is configured as follows:
    - TX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - ADC/ADC_AnalogWindowWatchdog/Source/main.c                Main program
  - ADC/ADC_AnalogWindowWatchdog/Source/g32f031_int.c         Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
