/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    pThread skeleton implementation for PolarSys rover
 *
 *    Configuration file is located at /opt/rover-app/config/rover_config.txt
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *    Gael Blondelle - API functions
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *
 */

#include <roverapp.h>

#include <libraries/pthread_distribution_lib/pthread_distribution.h>
#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <libraries/timing/timing.h>

#include <tasks/ultrasonic_sensor_grove_task.h>
#include <tasks/temperature_task.h>
#include <tasks/keycommand_task.h>
#include <tasks/motordriver_task.h>
#include <tasks/infrared_distance_task.h>
#include <tasks/display_sensors_task.h>
#include <tasks/compass_sensor_task.h>
#include <tasks/record_timing_task.h>
#include <tasks/ultrasonic_sensor_sr04_front_task.h>
#include <tasks/ultrasonic_sensor_sr04_back_task.h>
#include <tasks/adaptive_cruise_control_task.h>
#include <tasks/parking_task.h>
#include <tasks/hono_interaction_task.h>
#include <tasks/cpu_logger_task.h>
#include <tasks/oled_task.h>
#include <tasks/srf02_task.h>
#include <tasks/bluetooth_task.h>
#include <tasks/external_gpio_task.h>
#include <tasks/image_processing_task.h>
#include <tasks/booth_modes_task.h>
#include <tasks/socket_client_task.h>
#include <tasks/socket_server_task.h>
#include <tasks/accelerometer_task.h>
#include <tasks/mqtt_publish_task.h>
#include <tasks/mqtt_subscribe_task.h>

#include <interfaces.h>
#include <signal.h>


#define CHECK_RET(ret) if (ret) return ret;

using namespace std;

/* Configuration struct */
rover_config rover_config_obj;

//Using rover namespace from Rover API
using namespace rover;

//Create global RoverBase object from Rover API
RoverBase r_base;
RoverDriving r_driving = RoverDriving();
RoverDisplay my_display;
RoverUtils r_utils;

/* Threads */
pthread_t ultrasonic_grove_thread;
pthread_t ultrasonic_sr04_front_thread;
pthread_t ultrasonic_sr04_back_thread;
pthread_t temperature_thread;
//	pthread_t keycommand_input_thread;
pthread_t motordriver_thread;
pthread_t infrared_thread;
pthread_t displaysensors_thread;
pthread_t compasssensor_thread;
pthread_t record_timing_thread;
pthread_t adaptive_cruise_control_thread;
pthread_t parking_thread;
pthread_t hono_interaction_thread;
pthread_t cpu_logger_thread;
pthread_t oled_thread;
pthread_t srf02_thread;
pthread_t bluetooth_thread;
pthread_t extgpio_thread;
pthread_t booth_thread;
pthread_t socket_client_thread;
pthread_t socket_server_thread;
pthread_t accelerometer_thread;
pthread_t mqtt_publish_thread;
pthread_t mqtt_subscribe_thread;

/* Timing interfaces for thread measurement */
timing_interface compass_task_ti;
timing_interface temperature_task_ti;
timing_interface display_sensors_task_ti;
timing_interface infrared_distance_task_ti;
timing_interface keycommand_task_ti;
timing_interface motordriver_task_ti;
timing_interface ultrasonic_grove_task_ti;
timing_interface ultrasonic_sr04_front_task_ti;
timing_interface ultrasonic_sr04_back_task_ti;
timing_interface compass_sensor_task_ti;
timing_interface acc_task_ti;
timing_interface record_timing_task_ti;
timing_interface parking_task_ti;
timing_interface hono_task_ti;
timing_interface cpu_logger_task_ti;
timing_interface oled_task_ti;
timing_interface srf02_task_ti;
timing_interface bluetooth_task_ti;
timing_interface extgpio_task_ti;
timing_interface imgproc_task_ti;
timing_interface booth_task_ti;
timing_interface socket_client_task_ti;
timing_interface socket_server_task_ti;
timing_interface accelerometer_task_ti;
timing_interface mqtt_publish_task_ti;
timing_interface mqtt_subscribe_task_ti;

//Shared data between threads

SharedData<float> temperature_shared;
SharedData<float> humidity_shared;
SharedData<int> distance_grove_shared;
SharedData<int> distance_sr04_front_shared;
SharedData<int> distance_sr04_back_shared;
SharedData<char>  keycommand_shared;
SharedData<float> bearing_shared;
SharedData<float> timing_shared;
SharedData<int> driving_mode;
SharedData<int> speed_shared;
SharedData<int> buzzer_status_shared;
SharedData<int> shutdown_hook_shared;
SharedData<int> display_use_elsewhere_shared;
SharedData<int> display_mode_shared;
/* For proper termination */
SharedData<int> running_flag;

AccelerometerData_t accelerometerdata_shared;
pthread_mutex_t accelerometerdata_lock;

SharedDataArray<double, 4> cpu_util_shared;
SharedDataArray<float, 4> infrared_shared;

pthread_mutex_t display_lock;

// For e.g. gpio, polling intensive operations with no delay
pthread_mutex_t gpio_intensive_operation_lock;

int main_running_flag = 1;

// Function to handle the joining of every threads
void joinThread(pthread_t * thread_to_join) {
	char * ret = nullptr;
	int thread_ret = 0;
	char thread_name[10] = {};

	// If there thread wasn't created
	if (!(*thread_to_join))
	{
		return;
	}

	pthread_getname_np(*thread_to_join, thread_name, 10);

	printf("Joining Thread %d\n", (uint) *thread_to_join);

	thread_ret  = pthread_join(*thread_to_join, (void **) &ret);
	if (thread_ret)
	{
		printf("Error joining thread %s - returning %d.\n", thread_name, thread_ret);
	}
}

// Function to create every thread
int createThread(pthread_t * thread_to_create, void *(*thread_funct) (void *), const char * name) {
	pthread_attr_t attrs;

	pthread_attr_init(&attrs);
	pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);

	printf("Creating Thread %s\n", name);

	if(pthread_create(thread_to_create, &attrs, thread_funct, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(*thread_to_create, name); //If name is too long, this function silently fails.
	}

	return 0;
}

// Singal handler
void exitHandler(int dummy)
{
	running_flag.set(0);

	joinThread(&ultrasonic_grove_thread);
	joinThread(&ultrasonic_sr04_back_thread);
	joinThread(&ultrasonic_sr04_front_thread);
	joinThread(&temperature_thread);
	joinThread(&motordriver_thread);
	joinThread(&displaysensors_thread);
	joinThread(&infrared_thread);
	joinThread(&compasssensor_thread);
	joinThread(&record_timing_thread);
	joinThread(&adaptive_cruise_control_thread);
	joinThread(&parking_thread);
	joinThread(&hono_interaction_thread);
	joinThread(&cpu_logger_thread);
	joinThread(&oled_thread);
	joinThread(&extgpio_thread);
	joinThread(&booth_thread);
	joinThread(&socket_client_thread);
	joinThread(&socket_server_thread);
	joinThread(&accelerometer_thread);
	joinThread(&mqtt_publish_thread);
	joinThread(&mqtt_subscribe_thread);

	main_running_flag = 0;
	return;
}

// Main function
int main()
{
	/* Read configuration from config file */
	rover_config_obj = getRoverConfig(ROVER_CONFIG_FILE);

	int ret = 0;
	r_base = RoverBase();
	my_display = RoverDisplay();
	r_utils = RoverUtils();

	//Initialize some of the rover components
	r_base.initialize();
	r_driving.initialize();
	my_display.initialize();


	/* Add signals to exit threads properly */
	signal(SIGINT, exitHandler);
	signal(SIGTERM, exitHandler);
	signal(SIGKILL, exitHandler);

	//Initialize shared data
	driving_mode = MANUAL;
	speed_shared = HIGHEST_SPEED;
	running_flag = 1;
	main_running_flag = 1;
	keycommand_shared = 'F';

	//Initialize mutexes
	pthread_mutex_init(&display_lock, NULL);
	pthread_mutex_init(&gpio_intensive_operation_lock, NULL);

	//Thread objects
	pthread_t main_thread = pthread_self();
	pthread_setname_np(main_thread, "main_thread");

	//Thread creation
	/**/
	if (rover_config_obj.USE_GROOVE_SENSOR_C == 1)
	{
		ret = createThread(&ultrasonic_grove_thread, Ultrasonic_Sensor_Grove_Task, "US_grove");
	}
	else
	{
		ret = createThread(&ultrasonic_sr04_back_thread, Ultrasonic_Sensor_SR04_Back_Task, "US_sr04_back");
	}
	CHECK_RET(ret);

	ret = createThread(&ultrasonic_sr04_front_thread, Ultrasonic_Sensor_SR04_Front_Task, "US_sr04_front");
	CHECK_RET(ret);


	ret = createThread(&temperature_thread, Temperature_Task, "temperature");
	CHECK_RET(ret);

	ret = createThread(&motordriver_thread, MotorDriver_Task, "motordriver");
	CHECK_RET(ret);
	/**/

	ret = createThread(&infrared_thread, InfraredDistance_Task, "infrared");
	CHECK_RET(ret);

	ret = createThread(&displaysensors_thread, DisplaySensors_Task, "displaysensors");
	CHECK_RET(ret);
/**/
	ret = createThread(&compasssensor_thread, CompassSensor_Task, "compasssensor");
	CHECK_RET(ret);

	ret = createThread(&record_timing_thread, Record_Timing_Task, "record_timing");
	CHECK_RET(ret);

	ret = createThread(&adaptive_cruise_control_thread, Adaptive_Cruise_Control_Task, "acc");
	CHECK_RET(ret);

	ret = createThread(&parking_thread, Parking_Task, "parking");
	CHECK_RET(ret);

	ret = createThread(&hono_interaction_thread, Hono_Interaction_Task, "hono");
	CHECK_RET(ret);

	ret = createThread(&cpu_logger_thread, Cpu_Logger_Task, "cpulog");
	CHECK_RET(ret);

	ret = createThread(&oled_thread, OLED_Task, "oled");
	CHECK_RET(ret);

	ret = createThread(&extgpio_thread, External_GPIO_Task, "extg");
	CHECK_RET(ret);

	ret = createThread(&booth_thread, Booth_Modes_Task, "booth");
	CHECK_RET(ret);

	ret = createThread(&socket_client_thread, Socket_Client_Task, "SC");
	CHECK_RET(ret);

	ret = createThread(&socket_server_thread, Socket_Server_Task, "SS");
	CHECK_RET(ret);

	ret = createThread(&accelerometer_thread, Accelerometer_Task, "ACT");
	CHECK_RET(ret);

	ret = createThread(&mqtt_publish_thread, MQTT_Publish_Task, "MQTTP");
	CHECK_RET(ret);

	ret = createThread(&mqtt_subscribe_thread, MQTT_Subscribe_Task, "MQTTS");
	CHECK_RET(ret);/**/

	/* Set priority of a thread */
	//struct sched_param param ;
	//param.sched_priority = sched_get_priority_max (SCHED_RR) ;
	//pthread_setschedparam (temperature_thread, SCHED_RR, &param) ;

/**/
	/*if(pthread_create(&srf02_thread, NULL, SRF02_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(srf02_thread, "srf02"); //If name is too long, this function silently fails.
	}*/

	/*if(pthread_create(&bluetooth_thread, NULL, Bluetooth_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(bluetooth_thread, "ble"); //If name is too long, this function silently fails.
	}*/

	//Core pinning/mapping
/*	placeAThreadToCore (main_thread, 1);
	placeAThreadToCore (ultrasonic_sr04_front_thread, 2);
	if (rover_config_obj.USE_GROOVE_SENSOR_C == 1)
	{
		placeAThreadToCore (ultrasonic_grove_thread, 3);
	}
	else
	{
		placeAThreadToCore (ultrasonic_sr04_back_thread, 3);
	}
	placeAThreadToCore (temperature_thread, 3);
	placeAThreadToCore (compasssensor_thread, 0);
	placeAThreadToCore (motordriver_thread, 0);
	placeAThreadToCore (adaptive_cruise_control_thread, 0);
	placeAThreadToCore (parking_thread, 0);
	placeAThreadToCore (infrared_thread, 2);
	placeAThreadToCore (displaysensors_thread, 0);
	placeAThreadToCore (webserver_motordrive_thread, 0);
	*/

	/* Add signals to exit threads properly */
	signal(SIGINT, exitHandler);
	signal(SIGTERM, exitHandler);
	signal(SIGKILL, exitHandler);

	/* Use a different running flag to prevent deadlock because of receiving
	 * signal while having the lock  */
	while (main_running_flag)
	{
		//What main thread does should come here..
		// ...

		#if SIMULATOR
			usleep(1* SECONDS_TO_MICROSECONDS);
		#else
			delayMicroseconds(1* SECONDS_TO_MICROSECONDS);
		#endif

	}

	printf("Normally Exiting\n");

	pthread_exit(0);

	//Return 0
	return 0;
}
