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
int dimTemp = 0xf0;
int oldLightMode;
int newLightMode; 

enum LightState
{
		STATE_LIGHT_IDLE,
		STATE_LIGHT_ON,
		STATE_LIGHT_OFF,
		STATE_LIGHT_BACKW,
		STATE_LIGHT_BLINKR,
		STATE_LIGHT_BLINKL,
		STATE_LIGHT_DIMUP,
		STATE_LIGHT_DIMDOWN,
		STATE_LIGHT_AUTO,
		STATE_LIGHT_MAN,
		
	};

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

	r_light.autoLight = false;
	cout<< "LIGHT TASK IS RUNNING IN MANUAL " << endl;
	cout<< endl;
	char local_command = '5';
	LightState state = STATE_LIGHT_IDLE;
	while (running_flag.get())
	{
		light_task_tmr.recordStartTime();
		light_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		
		newLightMode = light_mode_shared.get();	
		switch (state)
		{

			case STATE_LIGHT_IDLE:
				//KEY= 0; do notghing
				if (newLightMode != oldLightMode) // each state shall be done just once per command push
				{
					switch (newLightMode)
					{
						case 0 : 
							state = STATE_LIGHT_IDLE;
							break;
						case 5 : 
							state = STATE_LIGHT_OFF;
							break;
						case 8 : 
							state = STATE_LIGHT_ON;
							break;
						case 2 : 
							state = STATE_LIGHT_BACKW;
							break;
						case 6 : 
							state = STATE_LIGHT_BLINKR;
							break;
						case 4 : 
							state = STATE_LIGHT_BLINKL;
							break;
						case 11 : 
							state = STATE_LIGHT_DIMUP;
							break;
						case 22 : 
							state = STATE_LIGHT_DIMDOWN;						
							break;
						case 10 : 
							state = STATE_LIGHT_AUTO;						
							break;
						case 20 : 
							state = STATE_LIGHT_MAN;						
						break;
						}
						
						
					oldLightMode = newLightMode; 
				}
				break;
			case STATE_LIGHT_OFF : // off
				//KEY= 5; 
				r_light.off();				
				state = STATE_LIGHT_IDLE;
				break;
			case STATE_LIGHT_ON: // on
				//KEY= 8; 
				r_light.dim();
				state = STATE_LIGHT_IDLE;	
				break;
			case STATE_LIGHT_BACKW: //backward
				//KEY= 2; 
				r_light.off();
				r_light.BackW();				
				state = STATE_LIGHT_IDLE;	
				break;
			case STATE_LIGHT_BLINKR:  //blink right
				//KEY= 6; 
				r_light.off();
				r_light.Blink_R();				
				if (newLightMode != 6) // the program will continue blinking till another comand comes.
				{
					state = STATE_LIGHT_IDLE;	
				}
				break;	
			case STATE_LIGHT_BLINKL:  // blink left
				//KEY= 4; 
				r_light.off();
				r_light.Blink_L();
				if (newLightMode != 4) // the program will continue blinking till another comand comes.
				{
					state = STATE_LIGHT_IDLE;	
				}
				break;
			case STATE_LIGHT_DIMDOWN:  // dime down
				//KEY= 22; 
				dimTemp = r_light.dimget() - 20;
				if (dimTemp < 15)  // down threshold
				{
					dimTemp = 15;
					}			
				r_light.dimset(dimTemp);				
				cout<< "in light task dim down to :  " << r_light.dimget() << endl;
				r_light.dim();	
				if ((newLightMode != 22) || (r_light.dimget() >= 255)) // dim down continuesly
				{
					state = STATE_LIGHT_IDLE;	
				}								
				break;
			case STATE_LIGHT_DIMUP:  //dime up
				//KEY= 11; 
				dimTemp = r_light.dimget() + 20;			
				if (dimTemp > 255) // up threshold
				{
					dimTemp = 255;
					}
				
				r_light.dimset(dimTemp);
				cout<< "in light task dim up to :  " << r_light.dimget() << endl;
				r_light.dim();	
				if ((newLightMode != 11) || (r_light.dimget() >= 255) ) //dim up continuesly
				{
					state = STATE_LIGHT_IDLE;	
				}
				break;
			case STATE_LIGHT_AUTO:  //AUTO MANUAL 
				r_light.autoLight = true;
				r_light.autoLight ? cout<< "Lights are in AUTO" << endl : cout<< "Lights are in MANUAL" << endl;
				state = STATE_LIGHT_IDLE;
				break;
			case STATE_LIGHT_MAN:  //AUTO MANUAL 
				r_light.autoLight = false;
				r_light.autoLight ? cout<< "Lights are in AUTO" << endl : cout<< "Lights are in MANUAL" << endl;
				state = STATE_LIGHT_IDLE;
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

	

	/* the function must return something - NULL will do */
	return NULL;
}
