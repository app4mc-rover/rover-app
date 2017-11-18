/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Key command obtainer function with pThreads
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
 */

#include <tasks/keycommand_task.h>

#include <ctime>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>
#include <roverapi/basic_psys_rover.h>

void *KeyCommandInput_Task(void * arg)
{
	timing keycommand_task_tmr;

	CollectThreadName("KeyCommandInput_Task");

	keycommand_task_tmr.setTaskID("KeyCommand");
	keycommand_task_tmr.setDeadline(0.2);
	keycommand_task_tmr.setPeriod(0.2);

	char keys;
	int running = 1;
	while (running)
	{
		keycommand_task_tmr.recordStartTime();
		keycommand_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		pthread_mutex_lock(&keycommand_lock);
			keys = getchar();
			if (isalpha(keys))
			{
				keycommand_shared = keys;
				//printf("Entered Command = %c\n",keycommand_shared);
			}
		pthread_mutex_unlock(&keycommand_lock);
		//Task content ends here -------------------------------------------------

		keycommand_task_tmr.recordEndTime();
		keycommand_task_tmr.calculateExecutionTime();
		keycommand_task_tmr.calculateDeadlineMissPercentage();
		keycommand_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&keycommand_task_ti_l);
			keycommand_task_ti.deadline = keycommand_task_tmr.getDeadline();
			keycommand_task_ti.deadline_miss_percentage = keycommand_task_tmr.getDeadlineMissPercentage();
			keycommand_task_ti.execution_time = keycommand_task_tmr.getExecutionTime();
			keycommand_task_ti.period = keycommand_task_tmr.getPeriod();
			keycommand_task_ti.prev_slack_time = keycommand_task_tmr.getPrevSlackTime();
			keycommand_task_ti.task_id = keycommand_task_tmr.getTaskID();
			keycommand_task_ti.start_time = keycommand_task_tmr.getStartTime();
			keycommand_task_ti.end_time = keycommand_task_tmr.getEndTime();
		pthread_mutex_unlock(&keycommand_task_ti_l);
		keycommand_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
