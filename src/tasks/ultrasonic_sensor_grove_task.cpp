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
 */

#include <tasks/ultrasonic_sensor_grove_task.h>

#include <ctime>
#include <pthread.h>

#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_grooveultrasonic.hpp>

void *Ultrasonic_Sensor_Grove_Task(void *unused)
{
	timing ultrasonic_grove_task_tmr;

	ultrasonic_grove_task_tmr.setTaskID("GrooveUltrasonic");
	ultrasonic_grove_task_tmr.setDeadline(0.2);
	ultrasonic_grove_task_tmr.setPeriod(0.2);

	RoverGrooveUltrasonic r_groove = RoverGrooveUltrasonic(ROVER_REAR);
	r_groove.initialize();

	while (running_flag.get())
	{
		ultrasonic_grove_task_tmr.recordStartTime();
		ultrasonic_grove_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		/*pthread_mutex_lock(&distance_grove_lock);
			distance_grove_shared = getCM_GrooveUltrasonicRanger();
		pthread_mutex_unlock(&distance_grove_lock);*/
#ifndef SIMULATOR
#if defined(USE_GROOVE_SENSOR)
		pthread_mutex_lock(&gpio_intensive_operation_lock);
			distance_sr04_back_shared = (int) r_groove.read();
		pthread_mutex_unlock(&gpio_intensive_operation_lock);
#endif
#endif
		//printf("Distance: %dcm\n", getCM_GrooveUltrasonicRanger());
		//Task content ends here -------------------------------------------------

		ultrasonic_grove_task_tmr.recordEndTime(); //!!!
		ultrasonic_grove_task_tmr.calculateExecutionTime();
		ultrasonic_grove_task_tmr.calculateDeadlineMissPercentage();
		ultrasonic_grove_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&ultrasonic_grove_task_ti.mutex);
			ultrasonic_grove_task_ti.deadline = ultrasonic_grove_task_tmr.getDeadline();
			ultrasonic_grove_task_ti.deadline_miss_percentage = ultrasonic_grove_task_tmr.getDeadlineMissPercentage();
			ultrasonic_grove_task_ti.execution_time = ultrasonic_grove_task_tmr.getExecutionTime();
			ultrasonic_grove_task_ti.period = ultrasonic_grove_task_tmr.getPeriod();
			ultrasonic_grove_task_ti.prev_slack_time = ultrasonic_grove_task_tmr.getPrevSlackTime();
			ultrasonic_grove_task_ti.task_id = ultrasonic_grove_task_tmr.getTaskID();
			ultrasonic_grove_task_ti.start_time = ultrasonic_grove_task_tmr.getStartTime();
			ultrasonic_grove_task_ti.end_time = ultrasonic_grove_task_tmr.getEndTime();
		pthread_mutex_unlock(&ultrasonic_grove_task_ti.mutex); //!!
		ultrasonic_grove_task_tmr.sleepToMatchPeriod();
	}

	//the function must return something - NULL will do
	return NULL;
}
