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

template <typename T>
class SharedData {
public:

		SharedData() {
				pthread_mutex_init(&this->lock, NULL);
		}

		~SharedData() {
				pthread_mutex_destroy(&this->lock);
		}

		T get() {
			T tmp;
			//pthread_mutex_lock(&this->lock);
			tmp = this->data;
			//pthread_mutex_unlock(&this->lock);

			return tmp;
		}

		void set(T data) {
				pthread_mutex_lock(&this->lock);
				this->data = data;
				pthread_mutex_unlock(&this->lock);
		}

		void operator= (T data) {
				this->set(data);
		}

private:
		pthread_mutex_t lock;
		T data;
};

extern SharedData<float> temperature_shared;
extern SharedData<float> humidity_shared;
extern SharedData<int> distance_grove_shared;
extern SharedData<char> keycommand_shared;
extern SharedData<float> bearing_shared;
extern SharedData<int> distance_sr04_back_shared;
extern SharedData<int> distance_sr04_front_shared;
extern SharedData<int> driving_mode;
extern SharedData<int> speed_shared;
extern SharedData<int> buzzer_status_shared;
extern SharedData<int> shutdown_hook_shared;
extern SharedData<int> display_use_elsewhere_shared;
extern SharedData<int> running_flag;

extern float infrared_shared[4];
extern pthread_mutex_t infrared_lock;

extern double cpu_util_shared[4];
extern pthread_mutex_t cpu_util_shared_lock;

/* For proper termination */
extern RoverBase r_base;
extern RoverDriving r_driving;
extern RoverDisplay my_display;
extern RoverUtils r_utils;

#endif /* MUTEXES_H_ */
