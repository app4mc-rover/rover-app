/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Booth modes implemented for demonstration of the APP4MC-APPSTACLE Rover
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 23.10.2017
 *
 */

#include <tasks/booth_modes_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>
#include <softPwm.h>

#include <roverapp.h>

void *Booth_Modes_Task(void * arg)
{
	timing booth_task_tmr;
	booth_task_tmr.setTaskID("BM");
	booth_task_tmr.setDeadline(0.1);
	booth_task_tmr.setPeriod(0.1);

	while (1)
	{
		booth_task_tmr.recordStartTime();
		booth_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		if (driving_mode.get() == BOOTH1)
		{

		}

		if (driving_mode.get() == BOOTH2)
		{

		}

		//Task content ends here -------------------------------------------------

		booth_task_tmr.recordEndTime();
		booth_task_tmr.calculateExecutionTime();
		booth_task_tmr.calculateDeadlineMissPercentage();
		booth_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&booth_task_ti.mutex);
			booth_task_ti.deadline = booth_task_tmr.getDeadline();
			booth_task_ti.deadline_miss_percentage = booth_task_tmr.getDeadlineMissPercentage();
			booth_task_ti.execution_time = booth_task_tmr.getExecutionTime();
			booth_task_ti.period = booth_task_tmr.getPeriod();
			booth_task_ti.prev_slack_time = booth_task_tmr.getPrevSlackTime();
			booth_task_ti.task_id = booth_task_tmr.getTaskID();
			booth_task_ti.start_time = booth_task_tmr.getStartTime();
			booth_task_ti.end_time = booth_task_tmr.getEndTime();
		pthread_mutex_unlock(&booth_task_ti.mutex);
		booth_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
