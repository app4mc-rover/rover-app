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
#include <wiringPi.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>
#include <mcp3004.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <RaspberryTest.h>

void setupInfraredSensors()
{
	// Init the analog digital converter
		  mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels
}

float getDistanceFromInfraredSensor(int channel){
	float x;
	float y=analogRead (BASE+channel);

// 1/cm to output voltage is almost linear between
// 80cm->0,4V->123
// 6cm->3,1V->961
// => y=5477*x+55 => x= (y-55)/5477
	if (y<123){
		x=100.00;
	} else {
		float inverse = (y-55)/5477;
		//printf("inverse=%f\n",inverse);
	// x is the distance in cm
		x = 1/inverse;
	}

    //printf("Distance channel row data %d:%f\n",channel,y);
    //printf("Distance channel (cm) %d:%f\n",channel,x);

	return x;
}

void *InfraredDistance_Task (void * arg)
{
	timing infrared_distance_task_tmr;

	CollectThreadName("InfraredDistance_Task");

	infrared_distance_task_tmr.setTaskID("Infrared");
	infrared_distance_task_tmr.setDeadline(0.5);
	infrared_distance_task_tmr.setPeriod(0.5);

	//setupInfraredSensors();
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
				infrared_shared[chan] = getDistanceFromInfraredSensor(chan);
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

