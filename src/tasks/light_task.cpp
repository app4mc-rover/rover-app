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

#include <tasks/light_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>
#include <roverapp.h>

/*
void ExitAutomaticModes(void)
{
	switch(driving_mode.get())
	{
		case ACC:
		case PARKING_LEFT:
		case PARKING_RIGHT:
		case BOOTH1:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			driving_mode = MANUAL;
			break;
		default:
			;// Do nothing
	}
}

void ManualModeSet(void)
{
	switch(driving_mode.get())
	{
		case ACC:
		case PARKING_LEFT:
		case PARKING_RIGHT:
		case BOOTH1:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			break;
		default:
			;// Do nothing
	}

	driving_mode = MANUAL;
	keycommand_shared = 'F';

}

void ParkingRightModeSet(void)
{
	switch(driving_mode.get())
	{
		case ACC:
		case MANUAL:
		case BOOTH1:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			break;
		default:
			;// Do nothing
	}

	driving_mode = PARKING_RIGHT;
	keycommand_shared = 'F';

}

void ParkingLeftModeSet(void)
{
	switch(driving_mode.get())
	{
		case ACC:
		case MANUAL:
		case BOOTH1:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			break;
		default:
			;// Do nothing
	}

	driving_mode = PARKING_LEFT;
	keycommand_shared = 'F';

}

void ACCModeSet(void)
{

	switch(driving_mode.get())
	{
		case PARKING_LEFT:
		case PARKING_RIGHT:
		case MANUAL:
		case BOOTH1:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			break;
		default:
			;// Do nothing
	}

	driving_mode = ACC;
	keycommand_shared = 'F';
}

void BoothMode1Set(void)
{

	switch(driving_mode.get())
	{
		case PARKING_LEFT:
		case PARKING_RIGHT:
		case MANUAL:
		case ACC:
		case BOOTH2:
			r_driving.stopRover(); //Stop the rover first.
			break;
		default:
			;// Do nothing
	}

	driving_mode = BOOTH1;
	keycommand_shared = 'F';
}



void BlinkMode(void)
{

	switch(light_mode.get())
	{
		case LIGHT_ON:
		case LIGHT_OFF:
		case LIGHT_BLINKR:
		case LIGHT_BLINKL:
		case LIGHT_BACKW:
			r_driving.stopRover(); //Stop the rover first.
			break;
		case LIGHT_FLASH:
		default:		
			;// Do nothing
	}

	driving_mode = BOOTH2;
	keycommand_shared = 'F';
}
*/


void *Light_Task(void * arg)
{
	timing light_task_tmr;

	light_task_tmr.setTaskID((char*)"LightSystem");
	light_task_tmr.setDeadline(0.1);
	light_task_tmr.setPeriod(0.1);

	int running = 1;
	
	char local_command = '5';

	while (running)// && running_flag.get())
	{
		light_task_tmr.recordStartTime();
		light_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		local_command = keycommand_shared.get();
		switch (local_command)
		{
			/*case 'G':
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
				r_driving.setSpeed(speed_shared.get());
				r_driving.goForward();
				break;
			case 'D':
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnBackwardRight();
				break;
			case 'S':
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.goBackward();
				break;
			case 'A':
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnBackwardLeft();
				break;
			case 'Q':
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnForwardLeft();
				break;
			case 'E':
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnForwardRight();
				break;
			case 'K':  //turn right on spot
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnRight();
				break;
			case 'J': //turn left on spot
				ExitAutomaticModes();
				r_driving.setSpeed(speed_shared.get());
				r_driving.turnLeft();
				break;
			case 'U':
				//Calibration mode
				break;
			case 'R':
				//Shutdown hook from web server
				r_base.shutdown();
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
				if (driving_mode.get() == MANUAL)
					r_driving.stopRover();
				break;*/
			case '0':
				running = 0;
				break;
			case '5':
					r_light.off();
				break;
			case '8':
					r_light.on();
				break;
			case '2':
					r_light.BackW();
				break;
			case '6':
					r_light.Blink_R();
				break;	
			case '4':
					r_light.Blink_L();
				break;
		}
		//Task content ends here -------------------------------------------------

		light_task_tmr.recordEndTime();
		light_task_tmr.calculateExecutionTime();
		light_task_tmr.calculateDeadlineMissPercentage();
		light_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&motordriver_task_ti.mutex);
			light_task_ti.deadline = light_task_tmr.getDeadline();
			light_task_ti.deadline_miss_percentage = light_task_tmr.getDeadlineMissPercentage();
			light_task_ti.execution_time = light_task_tmr.getExecutionTime();
			light_task_ti.period = light_task_tmr.getPeriod();
			light_task_ti.prev_slack_time = light_task_tmr.getPrevSlackTime();
			light_task_ti.task_id = light_task_tmr.getTaskID();
			light_task_ti.start_time = light_task_tmr.getStartTime();
			light_task_ti.end_time = light_task_tmr.getEndTime();
		pthread_mutex_unlock(&motordriver_task_ti.mutex);
		light_task_tmr.sleepToMatchPeriod();

	}

	r_light.~RoverLight(); 

	/* the function must return something - NULL will do */
	return NULL;
}
