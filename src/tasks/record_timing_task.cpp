/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Displays timing statistics of threads
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *
 * Update History:
 *    23.03.2017 - first revision
 *
 */

#include <tasks/record_timing_task.h>

#include <wiringPi.h>
#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <RaspberryTest.h>


void PrintTimingInfo(timing_interface ifc)
{
	if (ifc.execution_time != 0){
		printf("\tSTART=%1.4f\tEND=%1.4f\tDL=%1.4f,\tDL_MISS=%3d,\tEXEC=%1.4f\tPER=%1.4f\tSLACK=%1.4f\tID=%s\n",
															   ifc.start_time,
															   ifc.end_time,
															   ifc.deadline,
															   ifc.deadline_miss_percentage,
															   ifc.execution_time,
															   ifc.period,
															   ifc.prev_slack_time,
															   ifc.task_id);
	}
	else
	{
		printf("\tSTART=%1.4f\tEND=%1.4f\tDL=%1.4f,\tDL_MISS=%3d,\tEXEC=%1.4f\tPER=%1.4f\tSLACK=%1.4f\tID=(NotActive)\n", ifc.start_time,
				                                                       ifc.end_time,
																	   ifc.deadline,
																	   ifc.deadline_miss_percentage,
																	   ifc.execution_time,
																	   ifc.period,
																	   ifc.prev_slack_time);
	}
}

void *Record_Timing_Task (void * arg)
{
	timing record_timing_task_tmr;

	CollectThreadName("Record_Timing_Task");

	record_timing_task_tmr.setTaskID("RecordTiming");
	record_timing_task_tmr.setDeadline(0.5);
	record_timing_task_tmr.setPeriod(2.0);

	while (1)
	{
		record_timing_task_tmr.recordStartTime();
		record_timing_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		printf("---------- TASK TIMING/SCHEDULING INFO ----------\n");
		PrintTimingInfo(temperature_task_ti);
		PrintTimingInfo(display_sensors_task_ti);
		PrintTimingInfo(infrared_distance_task_ti);
		PrintTimingInfo(motordriver_task_ti);
		PrintTimingInfo(ultrasonic_grove_task_ti);
		PrintTimingInfo(ultrasonic_sr04_front_task_ti);
		PrintTimingInfo(ultrasonic_sr04_back_task_ti);
		PrintTimingInfo(record_timing_task_ti);
		PrintTimingInfo(acc_task_ti);
		PrintTimingInfo(parking_task_ti);
		PrintTimingInfo(compass_task_ti);
		PrintTimingInfo(oled_task_ti);
		PrintTimingInfo(srf02_task_ti);
		PrintTimingInfo(bluetooth_task_ti);
		PrintTimingInfo(extgpio_task_ti);
		PrintTimingInfo(booth_task_ti);
		PrintTimingInfo(imgproc_task_ti);
		PrintTimingInfo(socket_client_task_ti);
		PrintTimingInfo(socket_server_task_ti);
		printf("-------------------------------------------------\n");
		//Task content ends here -------------------------------------------------

		record_timing_task_tmr.recordEndTime();
		record_timing_task_tmr.calculateExecutionTime();
		record_timing_task_tmr.calculateDeadlineMissPercentage();
		record_timing_task_tmr.incrementTotalCycles();

		record_timing_task_ti.deadline = record_timing_task_tmr.getDeadline();
		record_timing_task_ti.deadline_miss_percentage = record_timing_task_tmr.getDeadlineMissPercentage();
		record_timing_task_ti.execution_time = record_timing_task_tmr.getExecutionTime();
		record_timing_task_ti.period = record_timing_task_tmr.getPeriod();
		record_timing_task_ti.prev_slack_time = record_timing_task_tmr.getPrevSlackTime();
		record_timing_task_ti.task_id = record_timing_task_tmr.getTaskID();
		record_timing_task_ti.start_time = record_timing_task_tmr.getStartTime();
		record_timing_task_ti.end_time = record_timing_task_tmr.getEndTime();

		record_timing_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}


