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

This example shows how to synchronize TMR peripherals.

  - ATMR is master for GTMR
    - Config PWM Mode
    - The update event is used as output Trigger
  - GTMR is slave for ATMR,
    - Config PWM Mode
    - The ITR0(ATMR) is used as input trigger

  - The ATMR peripherals clock is 64 MHz.

  - The Master Timer ATMR frequency :
    - ATMR frequency = (64M)/ (999 + 1) = 64 KHz
    - duty cycle = (499 + 1)/(999 + 1) = 50%.

  - The GTMR is running at:
    - (64 KHz)/ (999 + 1) = 64 Hz
    - duty cycle = (499 + 1)/(999 + 1) = 50%.

&par Hardware Description

  - using ATMR Channel 0 (PA10) to output signal.
  - using GTMR Channel 0 (PA7) to output signal.

&par Directory contents

  - TMR/TMR_Synchronize/Source/main.c               Main program
  - TMR/TMR_Synchronize/Source/g32f031_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on G32F031 EVB Devices.
