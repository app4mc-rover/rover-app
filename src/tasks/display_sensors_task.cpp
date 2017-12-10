/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Displaying Sensor Information with wiringPi and pThreads
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

#include <tasks/display_sensors_task.h>

#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_gy521.hpp>

void *DisplaySensors_Task (void * arg)
{
	timing display_sensors_task_tmr;

	display_sensors_task_tmr.setTaskID("Display-Sensors");
	display_sensors_task_tmr.setDeadline(3.0);
	display_sensors_task_tmr.setPeriod(3.0);

	RoverGY521 r_accel = RoverGY521();
	r_accel.initialize();

	while (running_flag.get())
	{
		display_sensors_task_tmr.recordStartTime();
		display_sensors_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		printf("Temperature: %f deg\n", temperature_shared.get());

		//delayMicroseconds(500000);

		printf("Humidity: %f percent\n", humidity_shared.get());

		//delayMicroseconds(500000);
		//printf("Distance(Groove): %d cm\n", distance_shared);

		printf("Distance(HCSR04Front): %d cm\n", distance_sr04_front_shared.get());
		printf("Distance(HCSR04Back): %d cm\n", distance_sr04_back_shared.get());

		printf("DistanceInfraredRearRight: %f cm\n", infrared_shared[0]);
		printf("DistanceInfraredRearLeft: %f cm\n", infrared_shared[1]);
		printf("DistanceInfraredFrontRight: %f cm\n", infrared_shared[2]);
		printf("DistanceInfraredFrontLeft: %f cm\n", infrared_shared[3]);

		printf("Bearing from QMC5883L/HMC5883L: %f\n", bearing_shared.get());

		printf("Accelerometer acceleration:\t%d\t\t%d\t\t%d\n", r_accel.getAccelX(), r_accel.getAccelY(), r_accel.getAccelZ());
		printf("Accelerometer angles:\t\t%f\t%f\t%f\n", r_accel.getAngleX(), r_accel.getAngleY(), r_accel.getAngleZ());

		printf("Bearing from accelerometer: %f\n", r_accel.read());

		//Task content ends here -------------------------------------------------
		display_sensors_task_tmr.recordEndTime();
		display_sensors_task_tmr.calculateExecutionTime();
		display_sensors_task_tmr.calculateDeadlineMissPercentage();
		display_sensors_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&display_sensors_task_ti.mutex);
			display_sensors_task_ti.deadline = display_sensors_task_tmr.getDeadline();
			display_sensors_task_ti.deadline_miss_percentage = display_sensors_task_tmr.getDeadlineMissPercentage();
			display_sensors_task_ti.execution_time = display_sensors_task_tmr.getExecutionTime();
			display_sensors_task_ti.period = display_sensors_task_tmr.getPeriod();
			display_sensors_task_ti.prev_slack_time = display_sensors_task_tmr.getPrevSlackTime();
			display_sensors_task_ti.task_id = display_sensors_task_tmr.getTaskID();
			display_sensors_task_ti.start_time = display_sensors_task_tmr.getStartTime();
			display_sensors_task_ti.end_time = display_sensors_task_tmr.getEndTime();
		pthread_mutex_unlock(&display_sensors_task_ti.mutex);
		display_sensors_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
