/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Temperature and Humidity Sensor DHT22 Task with wiringPi and pThreads
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

#include <tasks/temperature_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_dht22.hpp>

void *Temperature_Task(void *arg)
{
	timing temperature_task_tmr;

	temperature_task_tmr.setTaskID("DHT22");
	temperature_task_tmr.setDeadline(1);
	temperature_task_tmr.setPeriod(1);

	//Set priority
	//struct sched_param param ;
	//param.sched_priority = sched_get_priority_max (SCHED_RR) ;
	//pthread_setschedparam (pthread_self(), SCHED_RR, &param) ;

	float temperature, humidity;

	RoverDHT22 r_dht22 = RoverDHT22();
	r_dht22.initialize();

	while (running_flag.get())
	{
		temperature_task_tmr.recordStartTime();
		temperature_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
#ifndef SIMULATOR
		pthread_mutex_lock(&gpio_intensive_operation_lock);
			temperature = r_dht22.readTemperature();
		pthread_mutex_unlock(&gpio_intensive_operation_lock);

		temperature_shared.set(temperature);

		pthread_mutex_lock(&gpio_intensive_operation_lock);
			humidity = r_dht22.readHumidity();
		pthread_mutex_unlock(&gpio_intensive_operation_lock);

		humidity_shared = humidity;
#endif

		//Task content ends here -------------------------------------------------
		temperature_task_tmr.recordEndTime();
		temperature_task_tmr.calculateExecutionTime();
		temperature_task_tmr.calculateDeadlineMissPercentage();
		temperature_task_tmr.incrementTotalCycles();

		pthread_mutex_lock(&temperature_task_ti.mutex);
			temperature_task_ti.deadline = temperature_task_tmr.getDeadline();
			temperature_task_ti.deadline_miss_percentage = temperature_task_tmr.getDeadlineMissPercentage();
			temperature_task_ti.execution_time = temperature_task_tmr.getExecutionTime();
			temperature_task_ti.period = temperature_task_tmr.getPeriod();
			temperature_task_ti.prev_slack_time = temperature_task_tmr.getPrevSlackTime();
			temperature_task_ti.task_id = temperature_task_tmr.getTaskID();
			temperature_task_ti.start_time = temperature_task_tmr.getStartTime();
			temperature_task_ti.end_time = temperature_task_tmr.getEndTime();
		pthread_mutex_unlock(&temperature_task_ti.mutex);
		temperature_task_tmr.sleepToMatchPeriod();

	}

	/* the function must return something - NULL will do */
	return NULL;
}
