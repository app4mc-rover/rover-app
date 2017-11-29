/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    HCSR-04 ultrasonic sensor code with wiringPi and pThreads
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *   Resource: https://ninedof.wordpress.com/2013/07/16/rpi-hc-sr04-ultrasonic-sensor-mini-project/
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *    25.03.2017   -    timing library added
 *    27.03.2017   -    ultrasonic hc-sr04 sensor implementation corrected.
 *
 */

//Only necessary if you do not use logic level converter:
//A voltage divided should be used and it should be made sure that the grounds are not isolated and are connected to same line.
// ___________
//        +5V|----------------------------> Raspberry Pi +5V Pin
//           |
//           |
//       TRIG|----------------------------> Raspberry Pi BCM-24 Pin
//           |
//           |
//           |                           -----> Connect to Raspberry Pi  BCM-25 Pin
//  HC-SR04  |                           |
//           |                           |
//           |              330 ohm      |     470 ohm
//       ECHO|-------------^^^^^^^^^^--------^^^^^^^^^^--------> Connect to Raspberry Pi GND
//           |
//           |
//        GND|---------------------------------------------------> Connect to Raspberry Pi GND
// __________|


#ifndef TASKS_ULTRASONIC_SENSOR_SR04_FRONT_TASK_H_
#define TASKS_ULTRASONIC_SENSOR_SR04_FRONT_TASK_H_

#include <stdio.h>
#include <stdlib.h>

void *Ultrasonic_Sensor_SR04_Front_Task(void *);


#endif /* TASKS_ULTRASONIC_SENSOR_SR04_FRONT_TASK_H_ */
