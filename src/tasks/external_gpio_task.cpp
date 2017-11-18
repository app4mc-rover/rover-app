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
#include <tasks/oled_task.h>
#include <wiringPi.h>

#include <ctime>
#include <wiringPi.h>
#include <softTone.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>
#include <roverapi/basic_psys_rover.h>

void setupBuzzer (void)
{
	softToneCreate(BUZZER_PIN);
}

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
	if (digitalRead(SHUTDOWN_BUTTON_PIN) == LOW)
	{
		shutdownOSwithDisplay();
	}
}



void turnBuzzerOn (void)
{
	pthread_mutex_lock(&buzzer_status_shared_lock);
	buzzer_status_shared = 1;
	pthread_mutex_unlock(&buzzer_status_shared_lock);
}

void playShutdownTone (void)
{
	softToneWrite (BUZZER_PIN, BUZZER_SHUTDOWN_FREQ);
	delay(2000);
	softToneWrite (BUZZER_PIN, BUZZER_OFF_FREQ);
}

void playOnTone (void)
{
	softToneWrite (BUZZER_PIN, BUZZER_ON_FREQ);
	delay(1000);
	softToneWrite (BUZZER_PIN, BUZZER_OFF_FREQ);
}

void turnBuzzerOff (void)
{
	pthread_mutex_lock(&buzzer_status_shared_lock);
	buzzer_status_shared = 0;
	pthread_mutex_unlock(&buzzer_status_shared_lock);
}

/* Simply shuts down the operating system, preferrably after
 * OLED is done showing the indicator */
void shutdownOS (void)
{
	system("halt");
}

void *External_GPIO_Task(void *arg)
{
	timing extgpio_task_tmr;

	CollectThreadName("External_GPIO_Task");

	extgpio_task_tmr.setTaskID("GPIO");
	extgpio_task_tmr.setDeadline(0.2);
	extgpio_task_tmr.setPeriod(0.2);

	/* Setup Buzzer */
	setupBuzzer();

	while (1)
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
		pthread_mutex_lock(&extgpio_task_ti_l);
			extgpio_task_ti.deadline = extgpio_task_tmr.getDeadline();
			extgpio_task_ti.deadline_miss_percentage = extgpio_task_tmr.getDeadlineMissPercentage();
			extgpio_task_ti.execution_time = extgpio_task_tmr.getExecutionTime();
			extgpio_task_ti.period = extgpio_task_tmr.getPeriod();
			extgpio_task_ti.prev_slack_time = extgpio_task_tmr.getPrevSlackTime();
			extgpio_task_ti.task_id = extgpio_task_tmr.getTaskID();
			extgpio_task_ti.start_time = extgpio_task_tmr.getStartTime();
			extgpio_task_ti.end_time = extgpio_task_tmr.getEndTime();
		pthread_mutex_unlock(&extgpio_task_ti_l);
		extgpio_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
