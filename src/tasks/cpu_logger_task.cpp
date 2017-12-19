/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    CPU Utilization Logger Task for Rover / Raspberry Pi - uses external python script
 *
 * Note:
 *    read_core_usage.py must be installed to /opt/rover-app/scripts/read_core_usage.py
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 */

#include <tasks/cpu_logger_task.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

/* Signal header */
#include <signal.h>

#include <roverapp.h>

#include <roverapi/rover_utils.hpp>

void *Cpu_Logger_Task(void * arg) 
{

	timing cpu_logger_task_tmr;
	cpu_logger_task_tmr.setTaskID("CpuTsk");
	cpu_logger_task_tmr.setDeadline(3);
	cpu_logger_task_tmr.setPeriod(3);

	RoverUtils r_utils = RoverUtils();

	float util[4] = {};

	while (running_flag.get())
	{
		cpu_logger_task_tmr.recordStartTime();
		cpu_logger_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		r_utils.getCoreUtilization(util);
		cpu_util_shared.set(0, util[0]);
		cpu_util_shared.set(1, util[1]);
		cpu_util_shared.set(2, util[2]);
		cpu_util_shared.set(3, util[3]);

		//To debug:
		//printf ("%f %f %f %f",cpu_util_shared[0],cpu_util_shared[1],cpu_util_shared[2],cpu_util_shared[3]);

		//Task content ends here -------------------------------------------------

		cpu_logger_task_tmr.recordEndTime();
		cpu_logger_task_tmr.calculateExecutionTime();
		cpu_logger_task_tmr.calculateDeadlineMissPercentage();
		cpu_logger_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&cpu_logger_task_ti.mutex);
			cpu_logger_task_ti.deadline = cpu_logger_task_tmr.getDeadline();
			cpu_logger_task_ti.deadline_miss_percentage = cpu_logger_task_tmr.getDeadlineMissPercentage();
			cpu_logger_task_ti.execution_time = cpu_logger_task_tmr.getExecutionTime();
			cpu_logger_task_ti.period = cpu_logger_task_tmr.getPeriod();
			cpu_logger_task_ti.prev_slack_time = cpu_logger_task_tmr.getPrevSlackTime();
			cpu_logger_task_ti.task_id = cpu_logger_task_tmr.getTaskID();
			cpu_logger_task_ti.start_time = cpu_logger_task_tmr.getStartTime();
			cpu_logger_task_ti.end_time = cpu_logger_task_tmr.getEndTime();
		pthread_mutex_unlock(&cpu_logger_task_ti.mutex);
		cpu_logger_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
