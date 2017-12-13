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

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>

#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_button.hpp>

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
		r_base.shutdown();
	}

	if (user_b.readButton() == user_b.LO)
	{
		/* Debounce */

		#if SIMULATOR
			usleep(0.1 * SECONDS_TO_MICROSECONDS);
		#else
			delayMicroseconds(0.1* SECONDS_TO_MICROSECONDS);
		#endif

		if (user_b.readButton() == user_b.LO)
		{
			/* Adjust display mode and handle everything in the OLED task. */
			display_mode_shared = display_mode_shared.get() + 1;

			/* Wrap display_mode variable to started mode */
			if (display_mode_shared.get() == 2)
				display_mode_shared = 0;

			/* Wait a bit more to prevent double pushes */
			#if SIMULATOR
				usleep(0.1 * SECONDS_TO_MICROSECONDS);
			#else
				delayMicroseconds(0.1* SECONDS_TO_MICROSECONDS);
			#endif
		}
	}
#endif
}

void *External_GPIO_Task(void *arg)
{
	timing extgpio_task_tmr;

	extgpio_task_tmr.setTaskID("GPIO");
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
