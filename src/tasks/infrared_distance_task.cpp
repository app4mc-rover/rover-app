/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Infrared Distance Sensor Task with wiringPi and pThreads
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

#include <tasks/infrared_distance_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>

#include <roverapi/rover_infraredsensor.hpp>

void *InfraredDistance_Task (void * arg)
{
	timing infrared_distance_task_tmr;

	infrared_distance_task_tmr.setTaskID("Infrared");
	infrared_distance_task_tmr.setDeadline(0.5);
	infrared_distance_task_tmr.setPeriod(0.5);

	RoverInfraredSensor r_infrared0 = RoverInfraredSensor(ROVER_REAR_RIGHT);
	RoverInfraredSensor r_infrared1 = RoverInfraredSensor(ROVER_REAR_LEFT);
	RoverInfraredSensor r_infrared2 = RoverInfraredSensor(ROVER_FRONT_RIGHT);
	RoverInfraredSensor r_infrared3 = RoverInfraredSensor(ROVER_FRONT_LEFT);

	r_infrared0.initialize();
	r_infrared1.initialize();
	r_infrared2.initialize();
	r_infrared3.initialize();

	while (1)
	{
		infrared_distance_task_tmr.recordStartTime();
		infrared_distance_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		//Setting argument in pthread - whenever you R/W access to temperature_shared, you have to do the same.
		pthread_mutex_lock(&infrared_lock);
			infrared_shared[0] = r_infrared0.read();
			infrared_shared[1] = r_infrared1.read();
			infrared_shared[2] = r_infrared2.read();
			infrared_shared[3] = r_infrared3.read();
		pthread_mutex_unlock(&infrared_lock);
		//Task content ends here -------------------------------------------------

		infrared_distance_task_tmr.recordEndTime();
		infrared_distance_task_tmr.calculateExecutionTime();
		infrared_distance_task_tmr.calculateDeadlineMissPercentage();
		infrared_distance_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&infrared_distance_task_ti.mutex);
			infrared_distance_task_ti.deadline = infrared_distance_task_tmr.getDeadline();
			infrared_distance_task_ti.deadline_miss_percentage = infrared_distance_task_tmr.getDeadlineMissPercentage();
			infrared_distance_task_ti.execution_time = infrared_distance_task_tmr.getExecutionTime();
			infrared_distance_task_ti.period = infrared_distance_task_tmr.getPeriod();
			infrared_distance_task_ti.prev_slack_time = infrared_distance_task_tmr.getPrevSlackTime();
			infrared_distance_task_ti.task_id = infrared_distance_task_tmr.getTaskID();
			infrared_distance_task_ti.start_time = infrared_distance_task_tmr.getStartTime();
			infrared_distance_task_ti.end_time = infrared_distance_task_tmr.getEndTime();
		pthread_mutex_unlock(&infrared_distance_task_ti.mutex);
		infrared_distance_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
