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

template <typename T>
class SharedData {
public:

	SharedData() 
	{
		this->data = static_cast <T> (0);
		pthread_mutex_init(&this->lock, NULL);
	}

	~SharedData() 
	{
		pthread_mutex_destroy(&this->lock);
	}

	T get() 
	{
		T tmp = 0;
		pthread_mutex_lock(&this->lock);
		tmp = this->data;
		pthread_mutex_unlock(&this->lock);

		return tmp;
	}

	void set(T data) 
	{
		pthread_mutex_lock(&this->lock);
		this->data = data;
		pthread_mutex_unlock(&this->lock);
	}

	void operator= (T data) 
	{
		this->set(data);
	}

private:
	pthread_mutex_t lock;
	T data  = {};
};

template <typename T, int N>
class SharedDataArray {
public:

	SharedDataArray()
	{
		for (int i = 0; i < N; i++) 
		{
			this->data[i] = static_cast <T> (0);
		}
		pthread_mutex_init(&this->lock, NULL);
	}

	~SharedDataArray()
	{
		pthread_mutex_destroy(&this->lock);
	}

	T get(int i)
	{
		T tmp = 0;
		pthread_mutex_lock(&this->lock);
		tmp = this->data[i];
		pthread_mutex_unlock(&this->lock);

		return tmp;
	}

	void set(int i, T data)
	{
		pthread_mutex_lock(&this->lock);
		this->data[i] = data;
		pthread_mutex_unlock(&this->lock);
	}

private:
	T data[N] = {};
	pthread_mutex_t lock;
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

extern SharedData<int> display_mode_shared;

extern SharedDataArray<double, 4> cpu_util_shared;
extern SharedDataArray<float, 4> infrared_shared;

extern pthread_mutex_t display_lock;

extern pthread_mutex_t gpio_intensive_operation_lock;

extern pthread_mutex_t i2c_lock;

extern pthread_mutex_t mqtt_client_lock;


typedef struct
{
	int8_t gyro_x;
	int8_t gyro_y;
	int8_t gyro_z;
	float angle_x;
	float angle_y;
	float angle_z;
	int8_t accel_x;
	int8_t accel_y;
	int8_t accel_z;
} AccelerometerData_t;

extern AccelerometerData_t accelerometerdata_shared;
extern pthread_mutex_t accelerometerdata_lock;



#endif /* MUTEXES_H_ */
