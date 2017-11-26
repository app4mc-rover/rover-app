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

void *InfraredDistance_Task (void * arg)
{
	timing infrared_distance_task_tmr;

	infrared_distance_task_tmr.setTaskID("Infrared");
	infrared_distance_task_tmr.setDeadline(0.5);
	infrared_distance_task_tmr.setPeriod(0.5);

	int chan;

	while (1)
	{
		infrared_distance_task_tmr.recordStartTime();
		infrared_distance_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		//Setting argument in pthread - whenever you R/W access to temperature_shared, you have to do the same.
		pthread_mutex_lock(&infrared_lock);
			for (chan = 0; chan <= 3; chan ++)
			{
				infrared_shared[chan] = r.inRoverSensors().readInfraredSensor(chan);
			}
		pthread_mutex_unlock(&infrared_lock);
		//Task content ends here -------------------------------------------------

		infrared_distance_task_tmr.recordEndTime();
		infrared_distance_task_tmr.calculateExecutionTime();
		infrared_distance_task_tmr.calculateDeadlineMissPercentage();
		infrared_distance_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&infrared_distance_task_ti_l);
			infrared_distance_task_ti.deadline = infrared_distance_task_tmr.getDeadline();
			infrared_distance_task_ti.deadline_miss_percentage = infrared_distance_task_tmr.getDeadlineMissPercentage();
			infrared_distance_task_ti.execution_time = infrared_distance_task_tmr.getExecutionTime();
			infrared_distance_task_ti.period = infrared_distance_task_tmr.getPeriod();
			infrared_distance_task_ti.prev_slack_time = infrared_distance_task_tmr.getPrevSlackTime();
			infrared_distance_task_ti.task_id = infrared_distance_task_tmr.getTaskID();
			infrared_distance_task_ti.start_time = infrared_distance_task_tmr.getStartTime();
			infrared_distance_task_ti.end_time = infrared_distance_task_tmr.getEndTime();
		pthread_mutex_unlock(&infrared_distance_task_ti_l);
		infrared_distance_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}

