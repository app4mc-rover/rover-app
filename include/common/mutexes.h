/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Mutexes and global variables extern
 *
 * Authors:
 *    P. Cuadra,
 *    <pjcuadra@gmail.com>
 *
 * Contributors:
 *
 */

#ifndef MUTEXES_H_
#define MUTEXES_H_

#include <roverapi/rover_api.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_utils.hpp>

using namespace rover;

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

extern RoverBase r_base;
extern RoverDriving r_driving;
extern RoverDisplay my_display;
extern RoverUtils r_utils;

#endif /* MUTEXES_H_ */
