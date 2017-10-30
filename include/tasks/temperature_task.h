/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Temperature and Humidity Sensor DHT22 Task with wiringPi and pThreads - header file
 *    (Adapted from the tutorial: http://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi/)
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *    11.10.2017   -    re-compiled for DHT22 sensor
 *
*/

#ifndef TEMPERATURE_TASK_H_
#define TEMPERATURE_TASK_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_TIMINGS	85

#define DHT22_RPI_PIN   24 //BCM19, wiringPi 24

void *Temperature_Task(void * arg);


#endif /* TEMPERATURE_TASK_H_ */
