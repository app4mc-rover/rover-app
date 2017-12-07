/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Parking task - Source file
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>

 * Update History:
 *    06.06.2017   -    added initial task
 *
 */

#include <tasks/parking_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>

int StopParking (void)
{
	r_driving.stopRover();
	driving_mode = MANUAL;
	return 1;
}

void *Parking_Task(void * arg)
{
	timing parking_task_tmr;
	parking_task_tmr.setTaskID("Parking");
	parking_task_tmr.setDeadline(0.1);
	parking_task_tmr.setPeriod(0.1);

	float bearing_begin;

	while (1)
	{
		parking_task_tmr.recordStartTime();

		//Task content starts here -----------------------------------------------

		if (driving_mode.get() == PARKING_LEFT)
		{
			//printf ("PARKING STARTED\n");
			bearing_begin = bearing_shared.get();
			r_driving.setSpeed(speed_shared.get()-50);
			r_driving.turnLeft();
			/**
			 * The following bearing based parking does currently not work due to very unreliable bearing values
			 */
//			if (bearing_begin<91){
//				float upper_bound=360-abs(bearing_begin-90);
//				while((bearing_shared>0) && (bearing_shared<=bearing_begin));
//				while(bearing_shared>upper_bound);
//			}
//			else if (bearing_begin>=91 && bearing_begin<=120){
//				delay(2000);
//			}
//			else{
//				while(bearing_shared>=bearing_begin-90);
//			}
			delay(3000);


			r_driving.stopRover();
			r_driving.setSpeed(speed_shared.get());
			r_driving.goForward();
			delay(2000);
			r_driving.stopRover();

			//bearing_begin = bearing_shared;
			r_driving.setSpeed(speed_shared.get()-50);
			r_driving.turnRight();
			//while(((int)bearing_begin+(int)bearing_shared) % 360 <80);
			delay(3000);

			r_driving.stopRover();
			StopParking();
			//printf ("PARKING ENDED \n");
		}
		else if (driving_mode.get() == PARKING_RIGHT)
		{
			//printf ("PARKING STARTED\n");
			bearing_begin = bearing_shared.get();
			r_driving.setSpeed(speed_shared.get()-50);
			r_driving.turnRight();
			//while(((int)bearing_begin+(int)bearing_shared) % 360 <85);
			delay(3000);

			r_driving.setSpeed(speed_shared.get());
			r_driving.goForward();
			delay(2000);

			bearing_begin = bearing_shared.get();
			r_driving.setSpeed(speed_shared.get()-50);
			r_driving.turnLeft();
			delay(3000);

			r_driving.stopRover();
			StopParking();
			//printf ("PARKING ENDED\n");
		}

		//Task content ends here -------------------------------------------------

		parking_task_tmr.recordEndTime();
		parking_task_tmr.calculateExecutionTime();
		parking_task_tmr.calculateDeadlineMissPercentage();
		parking_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&parking_task_ti.mutex);
			parking_task_ti.deadline = parking_task_tmr.getDeadline();
			parking_task_ti.deadline_miss_percentage = parking_task_tmr.getDeadlineMissPercentage();
			parking_task_ti.execution_time = parking_task_tmr.getExecutionTime();
			parking_task_ti.period = parking_task_tmr.getPeriod();
			parking_task_ti.prev_slack_time = parking_task_tmr.getPrevSlackTime();
			parking_task_ti.task_id = parking_task_tmr.getTaskID();
			parking_task_ti.start_time = parking_task_tmr.getStartTime();
			parking_task_ti.end_time = parking_task_tmr.getEndTime();
		pthread_mutex_unlock(&parking_task_ti.mutex);
		parking_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
