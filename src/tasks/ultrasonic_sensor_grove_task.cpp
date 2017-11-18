/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Ultrasonic Distance Sensor Task with wiringPi and pThreads
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
 * Additional:
 * 	  Migrated from Groove Ultrasonic Sensor Python Library
 *
 */

#include <tasks/ultrasonic_sensor_grove_task.h>

#include <wiringPi.h>
#include <ctime>
#include <pthread.h>

#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>
#include <roverapi/basic_psys_rover.h>

void setup_GrooveUltrasonicRanger() {
	//wiringPiSetup();   //Since this can only be used once in a program, we do it in main and comment this.

}

int getCM_GrooveUltrasonicRanger()
{
	long startTime, stopTime, elapsedTime, distance = 0;
	pinMode(SIG, OUTPUT);
	digitalWrite(SIG, LOW);
	delayMicroseconds(2);
	digitalWrite(SIG, HIGH);
	delayMicroseconds(5);
	digitalWrite(SIG,LOW);
	pinMode(SIG,INPUT);

	startTime = micros();
	while (digitalRead(SIG) == LOW  );
	startTime = micros();
	// For values above 40cm, groove sensor is unable to receive signals which causes it to stuck
	// This is resolved by adding the timeout below.
	// However, this timeout cause values bigger than 40 to fluctuate
	while (digitalRead(SIG) == HIGH && micros() < startTime + 100000);
	stopTime = micros();
	elapsedTime = stopTime - startTime;
	distance = elapsedTime / 29 /2;
	// The below protection is to ensure there is no value fluctuation
	if (distance > 40 )
		distance = 40;
	return distance;
}


void *Ultrasonic_Sensor_Grove_Task(void *unused)
{
	timing ultrasonic_grove_task_tmr;

	CollectThreadName("Ultrasonic_Sensor_Grove_Task");

	ultrasonic_grove_task_tmr.setTaskID("GrooveUltrasonic");
	ultrasonic_grove_task_tmr.setDeadline(0.5);
	ultrasonic_grove_task_tmr.setPeriod(0.5);

	setup_GrooveUltrasonicRanger();
	while (1)
	{
		ultrasonic_grove_task_tmr.recordStartTime();
		ultrasonic_grove_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		/*pthread_mutex_lock(&distance_grove_lock);
			distance_grove_shared = getCM_GrooveUltrasonicRanger();
		pthread_mutex_unlock(&distance_grove_lock);*/
		pthread_mutex_lock(&distance_sr04_back_lock);
			distance_sr04_back_shared = getCM_GrooveUltrasonicRanger();
		pthread_mutex_unlock(&distance_sr04_back_lock);
		//printf("Distance: %dcm\n", getCM_GrooveUltrasonicRanger());
		//Task content ends here -------------------------------------------------

		ultrasonic_grove_task_tmr.recordEndTime(); //!!!
		ultrasonic_grove_task_tmr.calculateExecutionTime();
		ultrasonic_grove_task_tmr.calculateDeadlineMissPercentage();
		ultrasonic_grove_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&ultrasonic_grove_task_ti_l);
			ultrasonic_grove_task_ti.deadline = ultrasonic_grove_task_tmr.getDeadline();
			ultrasonic_grove_task_ti.deadline_miss_percentage = ultrasonic_grove_task_tmr.getDeadlineMissPercentage();
			ultrasonic_grove_task_ti.execution_time = ultrasonic_grove_task_tmr.getExecutionTime();
			ultrasonic_grove_task_ti.period = ultrasonic_grove_task_tmr.getPeriod();
			ultrasonic_grove_task_ti.prev_slack_time = ultrasonic_grove_task_tmr.getPrevSlackTime();
			ultrasonic_grove_task_ti.task_id = ultrasonic_grove_task_tmr.getTaskID();
			ultrasonic_grove_task_ti.start_time = ultrasonic_grove_task_tmr.getStartTime();
			ultrasonic_grove_task_ti.end_time = ultrasonic_grove_task_tmr.getEndTime();
		pthread_mutex_unlock(&ultrasonic_grove_task_ti_l); //!!
		ultrasonic_grove_task_tmr.sleepToMatchPeriod();
	}

	//the function must return something - NULL will do
	return NULL;
}
