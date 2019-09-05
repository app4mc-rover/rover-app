/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    External GPIO task for handling buttons and buzzer for Rover
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 11.10.2017
 *
*/

#include <tasks/external_gpio_task.h>

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>

#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_button.hpp>

//Keyboard
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <cmath>
#include <cerrno>
#include <cstdio>
#include <iostream> 
#include <stdio.h>  
#include <string.h>
#include <linux/input.h>
#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
using namespace std;


/* Checks global variable buzzer_status */
/* 1-> ON    0-> OFF */
void buzzerHandler (void)
{
	/* TODO: To be checked
	if ( buzzer_status_shared == 0 )
	{
		softToneWrite (BUZZER_PIN, BUZZER_OFF_FREQ);
	}
	else
	{
		softToneWrite (BUZZER_PIN, BUZZER_ON_FREQ);
	}


	if (distance_sr04_front_shared < 10)
	{
		softToneWrite (BUZZER_PIN, (int) ((10 - distance_sr04_front_shared)*500));
	}
	else
	{
		softToneWrite (BUZZER_PIN, BUZZER_OFF_FREQ);
	}
	*/
}
//keyboard
using namespace std;
int keyboardHandler (void)
{

	//char input = 'f';
	int FileDevice;
	int ReadDevice;
	int Index;
	struct input_event InputEvent[64];
	int version;
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	int input;

	//----- OPEN THE INPUT DEVICE -----
	if ((FileDevice = open("/dev/input/event1", O_RDONLY)) < 0)		//<<<<SET THE INPUT DEVICE PATH HERE
	{
		perror("KeyboardMonitor can't open input device");
		close(FileDevice);
		return 0; ///////////////
	}

	//----- GET DEVICE VERSION -----
	if (ioctl(FileDevice, EVIOCGVERSION, &version))
	{
		perror("KeyboardMonitor can't get version");
		close(FileDevice);
		return 0; ////////////
	}
	//printf("Input driver version is %d.%d.%d\n", version >> 16, (version >> 8) & 0xff, version & 0xff);

	//----- GET DEVICE INFO -----
	ioctl(FileDevice, EVIOCGID, id);
	//printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n", id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);
	
	memset(bit, 0, sizeof(bit));
	ioctl(FileDevice, EVIOCGBIT(0, EV_MAX), bit[0]);

	//----- READ KEYBOARD EVENTS -----

		ReadDevice = read(FileDevice, InputEvent, sizeof(struct input_event) * 64);

		if (ReadDevice < (int) sizeof(struct input_event))
		{
			//This should never happen
			//perror("KeyboardMonitor error reading - keyboard lost?");
			close(FileDevice);
			return 0; ////////////
		}
		else
		{
			for (Index = 0; Index < ReadDevice / sizeof(struct input_event); Index++)
			{
				//We have:
				//	InputEvent[Index].time		timeval: 16 bytes (8 bytes for seconds, 8 bytes for microseconds)
				//	InputEvent[Index].type		See input-event-codes.h
				//	InputEvent[Index].code		See input-event-codes.h
				//	InputEvent[Index].value		01 for keypress, 00 for release, 02 for autorepeat
				
				if (InputEvent[Index].type == EV_KEY)
				{
					if (InputEvent[Index].value == 2)
					{
						//This is an auto repeat of a held down key
						//cout << (int)(InputEvent[Index].code) << " Auto Repeat";
						//cout << endl;
					}
					else if (InputEvent[Index].value == 1)
					{
						//----- KEY DOWN -----
						//cout << (int)(InputEvent[Index].code) << " Key Down";		//input-event-codes.h
						//cout << endl;
						input=(int)(InputEvent[Index].code) ;
					}
					else if (InputEvent[Index].value == 0)
					{
						//----- KEY UP -----
						//cout << (int)(InputEvent[Index].code) << " Key Up";		//input-event-codes.h
						//cout << endl;
						input=(int)(InputEvent[Index].code) ;
						
					}
				}
			}
		}
	    cout<< "input key is : "<< input <<endl;
	
	return input;
}

void buttonHandler (void)
{
#ifndef DEBUG_WO_RSL
	RoverButton user_b = RoverButton (USER_BUTTON);
	RoverButton shutdown_b = RoverButton (SHUTDOWN_BUTTON);
	RoverBuzzer buzzer = RoverBuzzer();

	user_b.initialize();
	shutdown_b.initialize();
	buzzer.initialize();

	if (shutdown_b.readButton() == shutdown_b.LO)
	{
		display_use_elsewhere_shared = 1;
		pthread_mutex_lock(&display_lock);
		r_base.shutdown();
		pthread_mutex_unlock(&display_lock);
		display_use_elsewhere_shared = 0;
	}

	if (user_b.readButton() == user_b.LO)
	{
		/* Debounce */

		#if SIMULATOR
			usleep(0.005 * SECONDS_TO_MICROSECONDS);
		#else
			delayMicroseconds(0.005* SECONDS_TO_MICROSECONDS);
		#endif

		if (user_b.readButton() == user_b.LO)
		{
			/* Adjust display mode and handle everything in the OLED task. */
			display_mode_shared = display_mode_shared.get() + 1;

			/* Wrap display_mode variable to started mode */
			if (display_mode_shared.get() == 3)
				display_mode_shared = 0;

			/* Wait a bit more to prevent double pushes */
			#if SIMULATOR
				usleep(0.4 * SECONDS_TO_MICROSECONDS);
			#else
				delayMicroseconds(0.4* SECONDS_TO_MICROSECONDS);
			#endif
		}
	}
#endif
}

void *External_GPIO_Task(void *arg)
{
	int a=0;
	timing extgpio_task_tmr;

	extgpio_task_tmr.setTaskID((char*)"GPIO");
	extgpio_task_tmr.setDeadline(0.2);
	extgpio_task_tmr.setPeriod(0.2);

	while (running_flag.get())
	{
		extgpio_task_tmr.recordStartTime();
		extgpio_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		/* Handle buzzer operation */
		buzzerHandler();
		buttonHandler();
		a = keyboardHandler();
		cout<< "it receive------------------ : "<< a <<endl;
		switch (a) {
			case 72:
			case 9:
				light_mode_shared = 8;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			case 80:
			case 3:
				light_mode_shared = 2;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			case 77:
			case 7:
				light_mode_shared = 6;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			case 75:
			case 5:
				light_mode_shared = 4;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			case 76:
			case 6:
				light_mode_shared = 5;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			case 78:			// + key press
			case 27:			// + german keybord
				light_mode_shared = 11;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
				
			case 74:			// - key pres
			case 53:			// - german key board
				light_mode_shared = 22;
				cout<< "it select"<< light_mode_shared.get()<< "--"<<endl;
				break;
			default:
			//
			break;	
			
			}

		//Task content ends here -------------------------------------------------

		extgpio_task_tmr.recordEndTime();
		extgpio_task_tmr.calculateExecutionTime();
		extgpio_task_tmr.calculateDeadlineMissPercentage();
		extgpio_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&extgpio_task_ti.mutex);
			extgpio_task_ti.deadline = extgpio_task_tmr.getDeadline();
			extgpio_task_ti.deadline_miss_percentage = extgpio_task_tmr.getDeadlineMissPercentage();
			extgpio_task_ti.execution_time = extgpio_task_tmr.getExecutionTime();
			extgpio_task_ti.period = extgpio_task_tmr.getPeriod();
			extgpio_task_ti.prev_slack_time = extgpio_task_tmr.getPrevSlackTime();
			extgpio_task_ti.task_id = extgpio_task_tmr.getTaskID();
			extgpio_task_ti.start_time = extgpio_task_tmr.getStartTime();
			extgpio_task_ti.end_time = extgpio_task_tmr.getEndTime();
		pthread_mutex_unlock(&extgpio_task_ti.mutex);
		extgpio_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
