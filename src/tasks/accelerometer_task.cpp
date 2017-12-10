/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover accelerometer task
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created accelerometer task 10.12.2017
 *
 */

#include <tasks/accelerometer_task.h>

#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_gy521.hpp>

void *Accelerometer_Task (void * arg)
{
	timing accelerometer_task_tmr;

	accelerometer_task_tmr.setTaskID("Accelerometer");
	accelerometer_task_tmr.setDeadline(3.0);
	accelerometer_task_tmr.setPeriod(3.0);

	RoverGY521 r_accel = RoverGY521();
	r_accel.initialize();

	while (running_flag.get())
	{
		accelerometer_task_tmr.recordStartTime();
		accelerometer_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		pthread_mutex_lock(&accelerometerdata_lock);
			accelerometerdata_shared.accel_x = r_accel.getAccelX();
			accelerometerdata_shared.accel_y = r_accel.getAccelY();
			accelerometerdata_shared.accel_z = r_accel.getAccelZ();
			accelerometerdata_shared.gyro_x = r_accel.getGyroX();
			accelerometerdata_shared.gyro_y = r_accel.getGyroY();
			accelerometerdata_shared.gyro_z = r_accel.getGyroZ();
			accelerometerdata_shared.angle_x = r_accel.getAngleX();
			accelerometerdata_shared.angle_y = r_accel.getAngleY();
			accelerometerdata_shared.angle_z = r_accel.getAngleZ();
			accelerometerdata_shared.bearing = r_accel.read();
		pthread_mutex_unlock(&accelerometerdata_lock);
		//Task content ends here -------------------------------------------------

		accelerometer_task_tmr.recordEndTime();
		accelerometer_task_tmr.calculateExecutionTime();
		accelerometer_task_tmr.calculateDeadlineMissPercentage();
		accelerometer_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&accelerometer_task_ti.mutex);
			accelerometer_task_ti.deadline = accelerometer_task_tmr.getDeadline();
			accelerometer_task_ti.deadline_miss_percentage = accelerometer_task_tmr.getDeadlineMissPercentage();
			accelerometer_task_ti.execution_time = accelerometer_task_tmr.getExecutionTime();
			accelerometer_task_ti.period = accelerometer_task_tmr.getPeriod();
			accelerometer_task_ti.prev_slack_time = accelerometer_task_tmr.getPrevSlackTime();
			accelerometer_task_ti.task_id = accelerometer_task_tmr.getTaskID();
			accelerometer_task_ti.start_time = accelerometer_task_tmr.getStartTime();
			accelerometer_task_ti.end_time = accelerometer_task_tmr.getEndTime();
		pthread_mutex_unlock(&accelerometer_task_ti.mutex);
		accelerometer_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}

