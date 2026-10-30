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

This example describes how to use the ADC to sequential section sampling the voltage
applied to the ADC_CH0 and ADC_CH1.

ADC sequential section configuration as follows:
ADC SEQ  gap time:           GAPTIME_ADCCLK_0,
ADC SEQ  number:             2(SEQ1 and SEQ2),
ADC SEQ1 transmission times: 1(ADC_CH0),
ADC SEQ2 transmission times: 2(ADC_CH1, ADC_CH0),
ADC SEQ1 trigger source:     ATMR TRGO1(ATMR_TRGO_ENABLE),
ADC SEQ2 trigger source:     ATMR TRGO0(ATMR_TRGO_CC0IF).

The converted voltage is displayed on serial assistant through USART.

&par Hardware Description
 
using ADC_CH0(PA7)
using ADC_CH1(PA8)

The USART is configured as follows:
    - TX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

&par Directory contents

  - ADC/ADC_ADC_SequentialSectionSampling/Source/main.c                Main program
  - ADC/ADC_ADC_SequentialSectionSampling/Source/g32f031_int.c         Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
