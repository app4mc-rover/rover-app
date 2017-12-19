/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Hono interaction task for Rover
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 */

#include <tasks/hono_interaction_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_honocloud.hpp>

void *Hono_Interaction_Task(void * arg)
{
	timing hono_task_tmr;
	hono_task_tmr.setTaskID("HonoTsk");
	hono_task_tmr.setDeadline(2);
	hono_task_tmr.setPeriod(2);

	//Set-up hono instance attributes and register 4711 device to Hono
	RoverHonoCloud r_cloud = RoverHonoCloud();

	r_cloud.setHono("idial.institute", 8080, "DEFAULT_TENANT");

	r_cloud.setRegistrationPort(28080);
	r_cloud.registerDevice("4711");

	while (running_flag.get())
	{
		hono_task_tmr.recordStartTime();
		hono_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		// Send everything to Hono every second in this task using the following functions
		// TODO: This can be done with one curl command, probably a better way.
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverFront", distance_sr04_front_shared.get());
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverFrontLeft",infrared_shared.get(3));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverFrontRight", infrared_shared.get(2));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverRear", distance_sr04_back_shared.get());
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverRearLeft", infrared_shared.get(1));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverRearRight", infrared_shared.get(0));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverBearing", bearing_shared.get());
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverUtilCpu1", cpu_util_shared.get(0));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverUtilCpu2", cpu_util_shared.get(1));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverUtilCpu3", cpu_util_shared.get(2));
		r_cloud.sendTelemetry("4711","sensor1","hono-secret","roverUtilCpu4", cpu_util_shared.get(3));


		//Task content ends here -------------------------------------------------

		hono_task_tmr.recordEndTime();
		hono_task_tmr.calculateExecutionTime();
		hono_task_tmr.calculateDeadlineMissPercentage();
		hono_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&hono_task_ti.mutex);
			hono_task_ti.deadline = hono_task_tmr.getDeadline();
			hono_task_ti.deadline_miss_percentage = hono_task_tmr.getDeadlineMissPercentage();
			hono_task_ti.execution_time = hono_task_tmr.getExecutionTime();
			hono_task_ti.period = hono_task_tmr.getPeriod();
			hono_task_ti.prev_slack_time = hono_task_tmr.getPrevSlackTime();
			hono_task_ti.task_id = hono_task_tmr.getTaskID();
			hono_task_ti.start_time = hono_task_tmr.getStartTime();
			hono_task_ti.end_time = hono_task_tmr.getEndTime();
		pthread_mutex_unlock(&hono_task_ti.mutex);
		hono_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
