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
#include <string.h>
#include <wiringPi.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>
#include <softPwm.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>

/* Signal header */
#include <signal.h>

#include <roverapp.h>

/* Reads a certain file which returns core usage and exits with EX_OK or EX_SOFTWARE */
float* retrieveCoreUtilization (void)
{
	FILE *fp;
	char buffer[128];
	float util[5];
	size_t bytes_read;

	/* Execute the command */
	fp = popen("python /opt/rover-app/scripts/read_core_usage.py ","r");

	/* Read to buffer */
	bytes_read = fread(buffer, 1, sizeof(buffer), fp);

	if (bytes_read == 0 || bytes_read == sizeof(buffer))
		perror("Can't read from /opt/rover-app/scripts/read_core_usage.py");

	buffer[bytes_read] = '\0';

	//printf("buf:%s\n",buffer);

	/* Parse */
	sscanf(buffer,"[%f, %f, %f, %f]",&util[0], &util[1], &util[2], &util[3]);

	/* Return */
	return util;
}

void Cpu_Logger_Task_Terminator (int dummy)
{
	running_flag = 0;
}

void *Cpu_Logger_Task(void * arg)
{

	timing cpu_logger_task_tmr;
	cpu_logger_task_tmr.setTaskID("CpuTsk");
	cpu_logger_task_tmr.setDeadline(3);
	cpu_logger_task_tmr.setPeriod(3);

	CollectThreadName("Cpu_Logger_Task");

	/* Add termination signal handler to properly close fd */
	signal(SIGINT, Cpu_Logger_Task_Terminator);
	signal(SIGTERM, Cpu_Logger_Task_Terminator);
	signal(SIGKILL, Cpu_Logger_Task_Terminator);

	float *util;

	while (running_flag)
	{
		cpu_logger_task_tmr.recordStartTime();
		cpu_logger_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		util = retrieveCoreUtilization();
		pthread_mutex_lock(&cpu_util_shared_lock);
			cpu_util_shared[0] = util[0];
			cpu_util_shared[1] = util[1];
			cpu_util_shared[2] = util[2];
			cpu_util_shared[3] = util[3];
		pthread_mutex_unlock(&cpu_util_shared_lock);
		//To debug:
		//printf ("%f %f %f %f",cpu_util_shared[0],cpu_util_shared[1],cpu_util_shared[2],cpu_util_shared[3]);

		//Task content ends here -------------------------------------------------

		cpu_logger_task_tmr.recordEndTime();
		cpu_logger_task_tmr.calculateExecutionTime();
		cpu_logger_task_tmr.calculateDeadlineMissPercentage();
		cpu_logger_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&cpu_logger_task_ti_l);
			cpu_logger_task_ti.deadline = cpu_logger_task_tmr.getDeadline();
			cpu_logger_task_ti.deadline_miss_percentage = cpu_logger_task_tmr.getDeadlineMissPercentage();
			cpu_logger_task_ti.execution_time = cpu_logger_task_tmr.getExecutionTime();
			cpu_logger_task_ti.period = cpu_logger_task_tmr.getPeriod();
			cpu_logger_task_ti.prev_slack_time = cpu_logger_task_tmr.getPrevSlackTime();
			cpu_logger_task_ti.task_id = cpu_logger_task_tmr.getTaskID();
			cpu_logger_task_ti.start_time = cpu_logger_task_tmr.getStartTime();
			cpu_logger_task_ti.end_time = cpu_logger_task_tmr.getEndTime();
		pthread_mutex_unlock(&cpu_logger_task_ti_l);
		cpu_logger_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}


