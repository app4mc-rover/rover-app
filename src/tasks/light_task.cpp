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

#define BLINK_INTERVAL 2000

int blink_counter = 0;
char blink_mode = '0'; // Initial


void BlinkRight(void)
{
	switch (blink_mode)
	{
				
		case '1':
			if (blink_counter++ > BLINK_INTERVAL ) 
			{
			 blink_counter = 0;
			 blink_mode = '0';
			 r_light.off();
			}
			break;
		case '0':
			if (blink_counter++ > BLINK_INTERVAL )
			{
			 blink_mode = '1';
			 r_light.Blink_R_on();
			}
			break;
	}

}

void BlinkLeft(void)
{
	switch (blink_mode)
	{
				
		case '1':
			if (blink_counter++ > BLINK_INTERVAL ) 
			{
			 blink_counter = 0;
			 blink_mode = '0';
			 r_light.off();
			}
			break;
		case '0':
			if (blink_counter++ > BLINK_INTERVAL)
			{
			 blink_mode = '1';
			 r_light.Blink_L_on();
			}
			break;
	}

}

void *Light_Task(void * arg)
{
	timing light_task_tmr;

	light_task_tmr.setTaskID((char*)"LightSystem");
	light_task_tmr.setDeadline(0.1);
	light_task_tmr.setPeriod(0.1);

	int running = 1;
	cout<< "LIGHT tASK IS RUNNING " << endl;
	cout<< endl;
	
	char local_command = '5';
	
	while (running && running_flag.get())
	{
		light_task_tmr.recordStartTime();
		light_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		int a = light_mode_shared.get();
		
		switch (a)
		{

			case 0:
				running = 0;
				break;
			case 5 :
				r_light.off();
				cout<< "in light task ligh off selected " << endl;	
				break;
			case 8:
				r_light.on();
				blink_mode = 'I';
				cout<< "in light task light on selected " << endl;	
				break;
			case 2:
				r_light.off();
				r_light.BackW();
				cout<< "in light task light bakw selected " << endl;	
				break;
			case 6:
				r_light.off();
				r_light.Blink_R();
				cout<< "in light task Blink_R selected " << endl;	
				break;	
			case 4:
					r_light.off();
					r_light.Blink_L();
			case 11:				// dim up
					r_light.dim(255);
					cout<< "in light task dim up selected " << endl;
					//r_light.Blink_L();
			case 22:					// dim down
					r_light.dim(50);
					//r_light.Blink_L();
					cout<< "in light task dim down selected " << endl;
					
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

	//r_light.~RoverLight(); 

	/* the function must return something - NULL will do */
	return NULL;
}
