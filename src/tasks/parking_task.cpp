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

#include <wiringPi.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>
#include <roverapp.h>
#include <softPwm.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <tasks/motordriver_task.h>

int StopParking (void)
{
    stop();
    //Write to text file ..
    ofstream myfile;
    myfile.open("/var/www/html/ROVER_CMD.inc");
    myfile << "F";
    myfile.close();
    pthread_mutex_lock(&driving_mode_lock);
        driving_mode = MANUAL;
    pthread_mutex_unlock(&driving_mode_lock);
    return 1;
}

void *Parking_Task(void * arg)
{
	timing parking_task_tmr;
	parking_task_tmr.setTaskID("Parking");
	parking_task_tmr.setDeadline(0.1);
	parking_task_tmr.setPeriod(0.1);

	CollectThreadName("Parking_Task");

	float bearing_begin;

	while (1)
	{
		parking_task_tmr.recordStartTime();

		//Task content starts here -----------------------------------------------

		if (driving_mode == PARKING_LEFT)
		{
			bearing_begin = bearing_shared;
			turnOnSpot(FORWARD, LEFT, speed_shared-50);
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


			stop();
			go(FORWARD,speed_shared);
			delay(2000);
			stop();

			//bearing_begin = bearing_shared;
			turnOnSpot(FORWARD, RIGHT, speed_shared-50);
			//while(((int)bearing_begin+(int)bearing_shared) % 360 <80);
			delay(3000);

			stop();
			StopParking();
		}
		else if (driving_mode == PARKING_RIGHT)
		{
			bearing_begin = bearing_shared;
			turnOnSpot(FORWARD, RIGHT, speed_shared-50);
			//while(((int)bearing_begin+(int)bearing_shared) % 360 <85);
			delay(3000);

			go(FORWARD,speed_shared);
			delay(2000);

			bearing_begin = bearing_shared;
			turnOnSpot(FORWARD, LEFT, speed_shared-50);
			delay(3000);

			stop();
			StopParking();
		}

		//Task content ends here -------------------------------------------------

		parking_task_tmr.recordEndTime();
		parking_task_tmr.calculateExecutionTime();
		parking_task_tmr.calculateDeadlineMissPercentage();
		parking_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&parking_task_ti_l);
			parking_task_ti.deadline = parking_task_tmr.getDeadline();
			parking_task_ti.deadline_miss_percentage = parking_task_tmr.getDeadlineMissPercentage();
			parking_task_ti.execution_time = parking_task_tmr.getExecutionTime();
			parking_task_ti.period = parking_task_tmr.getPeriod();
			parking_task_ti.prev_slack_time = parking_task_tmr.getPrevSlackTime();
			parking_task_ti.task_id = parking_task_tmr.getTaskID();
			parking_task_ti.start_time = parking_task_tmr.getStartTime();
			parking_task_ti.end_time = parking_task_tmr.getEndTime();
		pthread_mutex_unlock(&parking_task_ti_l);
		parking_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}




