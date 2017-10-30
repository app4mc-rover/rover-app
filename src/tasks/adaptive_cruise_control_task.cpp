/*******************************************************************************
 * Copyright (c) 2017 FH-Dortmund and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     FH-Dortmund - initial API and implementation
 *******************************************************************************/

#include <tasks/adaptive_cruise_control_task.h>

#include <wiringPi.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>
#include <RaspberryTest.h>
#include <softPwm.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>

void *Adaptive_Cruise_Control_Task(void * arg)
{
	timing acc_task_tmr;
	acc_task_tmr.setTaskID("ACC");
	acc_task_tmr.setDeadline(0.1);
	acc_task_tmr.setPeriod(0.1);

	CollectThreadName("Adaptive_Cruise_Control_Task");

	while (1)
	{
		acc_task_tmr.recordStartTime();
		acc_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		if (driving_mode == ACC)
		{
			//front sensor -> distance_sr04_front_shared
			//back sensor -> distance_sr04_back_shared

			// front distance less than critical
			if (distance_sr04_front_shared < CRITICAL_DISTANCE)
			{
				// go back if distance ok
				if (distance_sr04_back_shared < CRITICAL_DISTANCE)
					stop();
				else
					go(BACKWARD, LOW_SPEED);
			}

			// front distance between correct and critical
			else if (distance_sr04_front_shared < CORRECT_DISTANCE && distance_sr04_front_shared >= CRITICAL_DISTANCE)
			{
				stop();
			}

			// front distance between safe and correct
			else if (distance_sr04_front_shared < SAFE_DISTANCE && distance_sr04_front_shared >= CORRECT_DISTANCE)
				go(FORWARD, LOW_SPEED);

			// distance > safe
			else if (distance_sr04_front_shared >= SAFE_DISTANCE)
				go(FORWARD, speed_shared);


//			printf("ACC mode is ON!\n");
//			printf("ACC infrared1: %d\n", (int)infrared_shared[1]);
		}

		//Task content ends here -------------------------------------------------

		acc_task_tmr.recordEndTime();
		acc_task_tmr.calculateExecutionTime();
		acc_task_tmr.calculateDeadlineMissPercentage();
		acc_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&acc_task_ti_l);
			acc_task_ti.deadline = acc_task_tmr.getDeadline();
			acc_task_ti.deadline_miss_percentage = acc_task_tmr.getDeadlineMissPercentage();
			acc_task_ti.execution_time = acc_task_tmr.getExecutionTime();
			acc_task_ti.period = acc_task_tmr.getPeriod();
			acc_task_ti.prev_slack_time = acc_task_tmr.getPrevSlackTime();
			acc_task_ti.task_id = acc_task_tmr.getTaskID();
			acc_task_ti.start_time = acc_task_tmr.getStartTime();
			acc_task_ti.end_time = acc_task_tmr.getEndTime();
		pthread_mutex_unlock(&acc_task_ti_l);
		acc_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}



