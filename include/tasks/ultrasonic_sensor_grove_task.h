/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Ultrasonic Distance Sensor Task with wiringPi and pThreads
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *
 * Additional:
 * 	  Migrated from Groove Ultrasonic Sensor Python Library
 *
 * 	  Pin for SIG input
 *      -> BCM-5,  Physical 29, wiringPi 21
 *
 */

#ifndef ULTRASONIC_SENSOR_GROVE_TASK_H_
#define ULTRASONIC_SENSOR_GROVE_TASK_H_

#include <stdio.h>
#include <stdlib.h>




#define SIG 2   //BCM-27   ->  WiringPi 2   //Same as ECHO1 pin, if some one wants to replace back sr04 with groove sensor

void *Ultrasonic_Sensor_Grove_Task(void *);
int getCM_GrooveUltrasonicRanger();
void setup_GrooveUltrasonicRanger();

#endif /* ULTRASONIC_SENSOR_GROVE_TASK_H_ */
