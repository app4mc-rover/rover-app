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
} timing_interface;

extern timing_interface compass_task_ti;
extern pthread_mutex_t compass_task_ti_l;

extern timing_interface temperature_task_ti;
extern pthread_mutex_t temperature_task_ti_l;

extern timing_interface display_sensors_task_ti;
extern pthread_mutex_t display_sensors_task_ti_l;

extern timing_interface infrared_distance_task_ti;
extern pthread_mutex_t infrared_distance_task_ti_l;

extern timing_interface keycommand_task_ti;
extern pthread_mutex_t keycommand_task_ti_l;

extern timing_interface motordriver_task_ti;
extern pthread_mutex_t motordriver_task_ti_l;

extern timing_interface ultrasonic_grove_task_ti;
extern pthread_mutex_t ultrasonic_grove_task_ti_l;

extern timing_interface ultrasonic_sr04_back_task_ti;
extern pthread_mutex_t  ultrasonic_sr04_back_task_ti_l;

extern timing_interface ultrasonic_sr04_front_task_ti;
extern pthread_mutex_t  ultrasonic_sr04_front_task_ti_l;

extern timing_interface compass_sensor_task_ti;
extern pthread_mutex_t compass_sensor_task_ti_l;

extern timing_interface acc_task_ti;
extern pthread_mutex_t acc_task_ti_l;

extern timing_interface record_timing_task_ti;

extern timing_interface parking_task_ti;
extern pthread_mutex_t parking_task_ti_l;

extern timing_interface hono_task_ti;
extern pthread_mutex_t hono_task_ti_l;

extern timing_interface cpu_logger_task_ti;
extern pthread_mutex_t cpu_logger_task_ti_l;

extern timing_interface oled_task_ti;
extern pthread_mutex_t oled_task_ti_l;

extern timing_interface srf02_task_ti;
extern pthread_mutex_t srf02_task_ti_l;

extern timing_interface bluetooth_task_ti;
extern pthread_mutex_t bluetooth_task_ti_l;

extern timing_interface extgpio_task_ti;
extern pthread_mutex_t extgpio_task_ti_l;

extern timing_interface imgproc_task_ti;
extern pthread_mutex_t imgproc_task_ti_l;

extern timing_interface booth_task_ti;
extern pthread_mutex_t booth_task_ti_l;

extern timing_interface socket_client_task_ti;
extern pthread_mutex_t socket_client_task_ti_l;

extern timing_interface socket_server_task_ti;
extern pthread_mutex_t socket_server_task_ti_l;

//---

extern float temperature_shared;
extern pthread_mutex_t temperature_lock;

extern float humidity_shared;
extern pthread_mutex_t humidity_lock;

extern int distance_grove_shared;
extern pthread_mutex_t distance_grove_lock;

extern char keycommand_shared;
extern pthread_mutex_t keycommand_lock;

extern float infrared_shared[4];
extern pthread_mutex_t infrared_lock;

extern float bearing_shared;
extern pthread_mutex_t compass_lock;

extern int distance_sr04_back_shared;
extern pthread_mutex_t distance_sr04_back_lock;

extern int distance_sr04_front_shared;
extern pthread_mutex_t distance_sr04_front_lock;

extern int driving_mode;
extern pthread_mutex_t driving_mode_lock;

extern int speed_shared;
extern pthread_mutex_t speed_lock;

extern double cpu_util_shared[4];
extern pthread_mutex_t cpu_util_shared_lock;

extern int buzzer_status_shared;
extern pthread_mutex_t buzzer_status_shared_lock;

extern int shutdown_hook_shared;

extern int display_use_elsewhere_shared;

/* For proper termination */
extern int running_flag;


#endif /* INTERFACES_H_ */
