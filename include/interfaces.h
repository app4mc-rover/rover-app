/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Interface file for shared resources
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
 */

#ifndef INTERFACES_H_
#define INTERFACES_H_

#include <stdio.h>
#include <common/mutexes.h>
#include <common/threads.h>

//Driving modes
#define MANUAL 0
#define ACC    1
#define PARKING_LEFT 2
#define PARKING_RIGHT 3
#define BOOTH1 4
#define BOOTH2 5

typedef struct  {
   char * task_id;
   int deadline_miss_percentage;
   double start_time;
   double end_time;
   double deadline;
   double period;
   double execution_time;
   double prev_slack_time;
   pthread_mutex_t mutex;
} timing_interface;

extern timing_interface compass_task_ti;

extern timing_interface temperature_task_ti;

extern timing_interface display_sensors_task_ti;

extern timing_interface infrared_distance_task_ti;

extern timing_interface keycommand_task_ti;

extern timing_interface motordriver_task_ti;

extern timing_interface ultrasonic_grove_task_ti;

extern timing_interface ultrasonic_sr04_back_task_ti;

extern timing_interface ultrasonic_sr04_front_task_ti;

extern timing_interface compass_sensor_task_ti;

extern timing_interface acc_task_ti;

extern timing_interface record_timing_task_ti;

extern timing_interface parking_task_ti;

extern timing_interface hono_task_ti;

extern timing_interface cpu_logger_task_ti;

extern timing_interface oled_task_ti;

extern timing_interface srf02_task_ti;

extern timing_interface bluetooth_task_ti;

extern timing_interface extgpio_task_ti;

extern timing_interface imgproc_task_ti;

extern timing_interface booth_task_ti;

extern timing_interface socket_client_task_ti;

extern timing_interface socket_server_task_ti;

extern timing_interface accelerometer_task_ti;

extern timing_interface mqtt_publish_task_ti;

extern timing_interface mqtt_subscribe_task_ti;

#endif /* INTERFACES_H_ */
