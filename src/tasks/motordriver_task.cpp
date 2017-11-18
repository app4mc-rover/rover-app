/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Motor driving Task with wiringPi and pThreads
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *    Gael Blondelle - API functions
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *
 */

#include <tasks/motordriver_task.h>
#include <tasks/oled_task.h>

#include <wiringPi.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>
#include <softPwm.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>
#include <roverapi/basic_psys_rover.h>

void ExitAutomaticModes(void)
{
	if (driving_mode == ACC || driving_mode == PARKING_LEFT || driving_mode == PARKING_RIGHT || driving_mode == BOOTH1 || driving_mode == BOOTH2)
	{
		stop(); //Stop the rover first.
		pthread_mutex_lock(&driving_mode_lock);
			driving_mode = MANUAL;
		pthread_mutex_unlock(&driving_mode_lock);
	}
}

void ManualModeSet(void)
{
	if (driving_mode == ACC || driving_mode == PARKING_LEFT || driving_mode == PARKING_RIGHT || driving_mode == BOOTH1 || driving_mode == BOOTH2)
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = MANUAL;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}

void ParkingRightModeSet(void)
{
	if (driving_mode == ACC || driving_mode == MANUAL || driving_mode == BOOTH1 || driving_mode == BOOTH2)
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = PARKING_RIGHT;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}

void ParkingLeftModeSet(void)
{
	if (driving_mode == ACC || driving_mode == MANUAL || driving_mode == BOOTH1 || driving_mode == BOOTH2)
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = PARKING_LEFT;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}

void ACCModeSet(void)
{
	if (driving_mode == PARKING_LEFT || driving_mode == PARKING_RIGHT || driving_mode == MANUAL || driving_mode == BOOTH1 || driving_mode == BOOTH2   )
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = ACC;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}

void BoothMode1Set(void)
{
	if (driving_mode == PARKING_LEFT || driving_mode == PARKING_RIGHT || driving_mode == MANUAL || driving_mode == BOOTH2  || driving_mode == ACC)
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = BOOTH1;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}

void BoothMode2Set(void)
{
	if (driving_mode == PARKING_LEFT || driving_mode == PARKING_RIGHT || driving_mode == MANUAL || driving_mode == BOOTH1 || driving_mode == ACC)
	{
		stop(); //Stop the rover first.
	}
	pthread_mutex_lock(&driving_mode_lock);
		driving_mode = BOOTH2;
	pthread_mutex_unlock(&driving_mode_lock);
	pthread_mutex_lock(&keycommand_lock);
		keycommand_shared = 'F';
	pthread_mutex_unlock(&keycommand_lock);
}


void *MotorDriver_Task(void * arg)
{
	timing motordriver_task_tmr;

	CollectThreadName("MotorDriver_Task");

	motordriver_task_tmr.setTaskID("MotorDriver");
	motordriver_task_tmr.setDeadline(0.1);
	motordriver_task_tmr.setPeriod(0.1);

	init();
	int running = 1;
	char local_command = 'f';

	//runside (LEFT, BACKWARD, FULL_SPEED);
	//runside (RIGHT, BACKWARD, FULL_SPEED);

	while (running)
	{
		motordriver_task_tmr.recordStartTime();
		motordriver_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		pthread_mutex_lock(&keycommand_lock);
			local_command = keycommand_shared;
			//printf("got=%c\n",keycommand_shared);
		pthread_mutex_unlock(&keycommand_lock);

		switch (local_command)
		{
			case 'G':
				running = 0;
				break;
			case 'P':
				ParkingLeftModeSet();
				break;
			case 'O':
				ParkingRightModeSet();
				break;
			case 'W':
				ExitAutomaticModes();
				go(FORWARD, speed_shared);
				break;
			case 'D':
				ExitAutomaticModes();
				turn(BACKWARD, LEFT, speed_shared);
				break;
			case 'S':
				ExitAutomaticModes();
				go(BACKWARD, speed_shared);
				break;
			case 'A':
				ExitAutomaticModes();
				turn(BACKWARD, RIGHT, speed_shared);
				break;
			case 'Q':
				ExitAutomaticModes();
				turn(FORWARD, RIGHT, speed_shared);
				break;
			case 'E':
				ExitAutomaticModes();
				turn(FORWARD, LEFT, speed_shared);
				break;
			case 'K':  //turn right on spot
				ExitAutomaticModes();
				turnOnSpot(FORWARD, RIGHT, speed_shared);
				break;
			case 'J': //turn left on spot
				ExitAutomaticModes();
				turnOnSpot(FORWARD, LEFT, speed_shared);
				break;
			case 'U':
				//Calibration mode
				break;
			case 'R':
				//Shutdown hook from web server
				shutdownOSwithDisplay();
				break;
			case 'M':
				//ACC mode set
				ACCModeSet();
				break;
			case 'X':
				//Manual mode set
				ManualModeSet();
				break;
			case 'L':
				//Booth mode (Demo 1) set
				BoothMode1Set();
				break;
			case 'N':
				//Booth mode (Demo 2) set
				BoothMode2Set();
				break;
			case 'F':
				stop();
				break;
		}
		//Task content ends here -------------------------------------------------

		motordriver_task_tmr.recordEndTime();
		motordriver_task_tmr.calculateExecutionTime();
		motordriver_task_tmr.calculateDeadlineMissPercentage();
		motordriver_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&motordriver_task_ti_l);
			motordriver_task_ti.deadline = motordriver_task_tmr.getDeadline();
			motordriver_task_ti.deadline_miss_percentage = motordriver_task_tmr.getDeadlineMissPercentage();
			motordriver_task_ti.execution_time = motordriver_task_tmr.getExecutionTime();
			motordriver_task_ti.period = motordriver_task_tmr.getPeriod();
			motordriver_task_ti.prev_slack_time = motordriver_task_tmr.getPrevSlackTime();
			motordriver_task_ti.task_id = motordriver_task_tmr.getTaskID();
			motordriver_task_ti.start_time = motordriver_task_tmr.getStartTime();
			motordriver_task_ti.end_time = motordriver_task_tmr.getEndTime();
		pthread_mutex_unlock(&motordriver_task_ti_l);
		motordriver_task_tmr.sleepToMatchPeriod();

	}

	/* the function must return something - NULL will do */
	return NULL;
}



