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
This example shows how to configure the ATMR peripheral to generate 3 different
signals with three different delays.

  - SystemCoreClock = 64MHz.
  - ATMR = SystemCoreClock = 64MHz. Prescaler = 63, ATMR counter clock = 1MHz
  - ATMR AutoReload = 999
  - ATMR CH0 pulse = 400
  - ATMR CH0 delay = 400/1MHz = 400us
  - ATMR CH1 pulse = 600
  - ATMR CH1 delay = 600/1MHz = 600us
  - ATMR CH2 pulse = 800
  - ATMR CH2 delay = 800/1MHz = 800us

&par Hardware Description

  - using ATMR Channel 1 (PA10) to output signal
  - using ATMR Channel 2 (PA11) to output signal
  - using ATMR Channel 3 (PA12) to output signal

&par Directory contents

  - TMR/TMR_OCInactive/Source/main.c               Main program
  - TMR/TMR_OCInactive/Source/g32f031_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
