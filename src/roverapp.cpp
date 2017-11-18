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
#include <libraries/hono_interaction/hono_interaction.h>
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

#include <interfaces.h>
#include <signal.h>
#include <roverapi/basic_psys_rover.h>

//Please comment the line below to work with SR-04 sensor instead of GROOVE for rear proximity sensing.
//#define USE_GROOVE_SENSOR 1

using namespace std;

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

/* Timing interfaces for thread measurement */
timing_interface compass_task_ti;
pthread_mutex_t compass_task_ti_l;

timing_interface temperature_task_ti;
pthread_mutex_t temperature_task_ti_l;

timing_interface display_sensors_task_ti;
pthread_mutex_t display_sensors_task_ti_l;

timing_interface infrared_distance_task_ti;
pthread_mutex_t infrared_distance_task_ti_l;

timing_interface keycommand_task_ti;
pthread_mutex_t keycommand_task_ti_l;

timing_interface motordriver_task_ti;
pthread_mutex_t motordriver_task_ti_l;

timing_interface ultrasonic_grove_task_ti;
pthread_mutex_t ultrasonic_grove_task_ti_l;

timing_interface ultrasonic_sr04_front_task_ti;
pthread_mutex_t  ultrasonic_sr04_front_task_ti_l;

timing_interface ultrasonic_sr04_back_task_ti;
pthread_mutex_t  ultrasonic_sr04_back_task_ti_l;

timing_interface compass_sensor_task_ti;
pthread_mutex_t compass_sensor_task_ti_l;

timing_interface acc_task_ti;
pthread_mutex_t acc_task_ti_l;

timing_interface record_timing_task_ti;

timing_interface parking_task_ti;
pthread_mutex_t parking_task_ti_l;

timing_interface hono_task_ti;
pthread_mutex_t hono_task_ti_l;

timing_interface cpu_logger_task_ti;
pthread_mutex_t cpu_logger_task_ti_l;

timing_interface oled_task_ti;
pthread_mutex_t oled_task_ti_l;

timing_interface srf02_task_ti;
pthread_mutex_t srf02_task_ti_l;

timing_interface bluetooth_task_ti;
pthread_mutex_t bluetooth_task_ti_l;

timing_interface extgpio_task_ti;
pthread_mutex_t extgpio_task_ti_l;

timing_interface imgproc_task_ti;
pthread_mutex_t imgproc_task_ti_l;

timing_interface booth_task_ti;
pthread_mutex_t booth_task_ti_l;

timing_interface socket_client_task_ti;
pthread_mutex_t socket_client_task_ti_l;

timing_interface socket_server_task_ti;
pthread_mutex_t socket_server_task_ti_l;

//Shared data between threads

float temperature_shared;
pthread_mutex_t temperature_lock;

float humidity_shared;
pthread_mutex_t humidity_lock;

int distance_grove_shared;
pthread_mutex_t distance_grove_lock;

int distance_sr04_front_shared;
pthread_mutex_t distance_sr04_front_lock;

int distance_sr04_back_shared;
pthread_mutex_t distance_sr04_back_lock;

char keycommand_shared;
pthread_mutex_t keycommand_lock;

float infrared_shared[4];
pthread_mutex_t infrared_lock;

float bearing_shared;
pthread_mutex_t compass_lock;

float timing_shared;
pthread_mutex_t timing_lock;

int driving_mode;
pthread_mutex_t driving_mode_lock;

int speed_shared;
pthread_mutex_t speed_lock;

double cpu_util_shared[4];
pthread_mutex_t cpu_util_shared_lock;

int buzzer_status_shared;
pthread_mutex_t buzzer_status_shared_lock;

int shutdown_hook_shared;

/* For proper termination */
int running_flag;

void exitHandler(int dummy)
{
	pthread_kill(ultrasonic_grove_thread, SIGTERM);
	pthread_kill(ultrasonic_sr04_front_thread, SIGTERM);
	pthread_kill(ultrasonic_sr04_back_thread, SIGTERM);
	pthread_kill(temperature_thread, SIGTERM);
	pthread_kill(motordriver_thread, SIGTERM);
	pthread_kill(infrared_thread, SIGTERM);
	pthread_kill(displaysensors_thread, SIGTERM);
	pthread_kill(compasssensor_thread, SIGTERM);
	pthread_kill(record_timing_thread, SIGTERM);
	pthread_kill(adaptive_cruise_control_thread, SIGTERM);
	pthread_kill(parking_thread, SIGTERM);
	pthread_kill(hono_interaction_thread, SIGTERM);
	pthread_kill(cpu_logger_thread, SIGTERM);
	pthread_kill(oled_thread, SIGTERM);
	pthread_kill(srf02_thread, SIGTERM);
	pthread_kill(bluetooth_thread, SIGTERM);
	pthread_kill(extgpio_thread, SIGTERM);
	pthread_kill(booth_thread, SIGTERM);
	pthread_kill(socket_client_thread, SIGTERM);
	pthread_kill(socket_server_thread, SIGTERM);
}

int main()
{
	//Register all the entries as devices to cloud
	registerEntriesToHonoInstance();

	/* Add signals to exit threads properly */
	signal(SIGINT, exitHandler);
	signal(SIGTERM, exitHandler);
	signal(SIGKILL, exitHandler);

	RefreshThreadList();

	CollectProcessID();

	CollectThreadName("Main_Thread");

	wiringPiSetup();

	//Initialize shared data
	temperature_shared = 0.0;
	humidity_shared = 0.0;
	distance_grove_shared = 0;
	distance_sr04_front_shared = 0;
	distance_sr04_back_shared = 0;
	keycommand_shared = 'f';
	infrared_shared[0] = 0.0;
	infrared_shared[1] = 0.0;
	infrared_shared[2] = 0.0;
	infrared_shared[3] = 0.0;
	bearing_shared = 0.0;
	driving_mode = MANUAL;
	speed_shared = FULL_SPEED;
	buzzer_status_shared = 0;
	shutdown_hook_shared = 0;
	running_flag = 1;

	//Initialize mutexes
	pthread_mutex_init(&temperature_lock, NULL);
	pthread_mutex_init(&humidity_lock, NULL);
	pthread_mutex_init(&distance_grove_lock, NULL);
	pthread_mutex_init(&distance_sr04_front_lock, NULL);
	pthread_mutex_init(&distance_sr04_back_lock, NULL);
	pthread_mutex_init(&keycommand_lock, NULL);
	pthread_mutex_init(&infrared_lock, NULL);
	pthread_mutex_init(&compass_lock, NULL);
	pthread_mutex_init(&driving_mode_lock, NULL);
	pthread_mutex_init(&buzzer_status_shared_lock, NULL);

	//Thread objects
	pthread_t main_thread = pthread_self();
	pthread_setname_np(main_thread, "main_thread");



	//Thread creation

#ifdef USE_GROOVE_SENSOR
	if(pthread_create(&ultrasonic_grove_thread, NULL, Ultrasonic_Sensor_Grove_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(ultrasonic_grove_thread, "US_grove"); //If name is too long, this function silently fails.
	}
#else
	if(pthread_create(&ultrasonic_sr04_back_thread, NULL, Ultrasonic_Sensor_SR04_Back_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(ultrasonic_sr04_back_thread, "US_sr04_back"); //If name is too long, this function silently fails.
	}
#endif

	if(pthread_create(&ultrasonic_sr04_front_thread, NULL, Ultrasonic_Sensor_SR04_Front_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(ultrasonic_sr04_front_thread, "US_sr04_front"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&temperature_thread, NULL, Temperature_Task, NULL)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}
	else
	{
		pthread_setname_np(temperature_thread, "temperature"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&motordriver_thread, NULL, MotorDriver_Task, NULL)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}
	else
	{
		pthread_setname_np(motordriver_thread, "motordriver"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&infrared_thread, NULL, InfraredDistance_Task, NULL)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}
	else
	{
		pthread_setname_np(infrared_thread, "infrared"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&displaysensors_thread, NULL, DisplaySensors_Task, NULL)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}
	else
	{
		pthread_setname_np(displaysensors_thread, "displaysensors"); //If name is too long, this function silently fails.
	}

	if (pthread_create(&compasssensor_thread, NULL, CompassSensor_Task, NULL)) {
			fprintf(stderr, "Error creating compass sensor thread\n");
			return 1;
	}
	else
	{
		pthread_setname_np(compasssensor_thread, "compasssensor"); //If name is too long, this function silently fails.
	}

	if (pthread_create(&record_timing_thread, NULL, Record_Timing_Task, NULL)) {
		fprintf(stderr, "Error creating compass sensor thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(record_timing_thread, "record_timing"); //If name is too long, this function silently fails.
	}

	if (pthread_create(&adaptive_cruise_control_thread, NULL, Adaptive_Cruise_Control_Task, NULL))
	{
		fprintf(stderr, "Error creating compass sensor thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(adaptive_cruise_control_thread, "acc"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&parking_thread, NULL, Parking_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(parking_thread, "parking"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&hono_interaction_thread, NULL, Hono_Interaction_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(hono_interaction_thread, "hono"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&cpu_logger_thread, NULL, Cpu_Logger_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(cpu_logger_thread, "cpulog"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&oled_thread, NULL, OLED_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(oled_thread, "oled"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&extgpio_thread, NULL, External_GPIO_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(extgpio_thread, "extg"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&booth_thread, NULL, Booth_Modes_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(booth_thread, "booth"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&socket_client_thread, NULL, Socket_Client_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(socket_client_thread, "SC"); //If name is too long, this function silently fails.
	}

	if(pthread_create(&socket_server_thread, NULL, Socket_Server_Task, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	else
	{
		pthread_setname_np(socket_server_thread, "SS"); //If name is too long, this function silently fails.
	}

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
#ifdef USE_GROOVE_SENSOR
	placeAThreadToCore (ultrasonic_grove_thread, 3);
#else
	placeAThreadToCore (ultrasonic_sr04_back_thread, 3);
#endif
	placeAThreadToCore (temperature_thread, 3);
	placeAThreadToCore (compasssensor_thread, 0);
	placeAThreadToCore (motordriver_thread, 0);
	placeAThreadToCore (adaptive_cruise_control_thread, 0);
	placeAThreadToCore (parking_thread, 0);
	placeAThreadToCore (infrared_thread, 2);
	placeAThreadToCore (displaysensors_thread, 0);
	placeAThreadToCore (webserver_motordrive_thread, 0);
	*/

	while (running_flag)
	{
		//What main thread does should come here..
		// ...
		delayMicroseconds(1* SECONDS_TO_MICROSECONDS);
	}
	pthread_exit(NULL);

	//Return 0
	return 0;
}

