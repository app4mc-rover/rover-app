/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Temperature and Humidity Sensor DHT22 Task with wiringPi and pThreads
 *    (Adapted from the tutorial: http://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi/)
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *    11.10.2017   -    re-compiled for DHT22 sensor
 *
*/

#include <tasks/temperature_task.h>

#include <ctime>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>

void readDHT22SensorData()
{
	int data[5] = { 0, 0, 0, 0, 0 };

	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode( DHT22_RPI_PIN, OUTPUT );
	digitalWrite( DHT22_RPI_PIN, LOW );
	delay( 18 );

	/* prepare to read the pin */
	pinMode( DHT22_RPI_PIN, INPUT );

	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHT22_RPI_PIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHT22_RPI_PIN );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];	// for DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];	// for DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}
		float f = c * 1.8f + 32;
		//printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );

		pthread_mutex_lock(&temperature_lock);
			temperature_shared = c;
		pthread_mutex_unlock(&temperature_lock);

		pthread_mutex_lock(&humidity_lock);
			humidity_shared = h;
		pthread_mutex_unlock(&humidity_lock);
	}
}

void *Temperature_Task(void *arg)
{
	timing temperature_task_tmr;

	CollectThreadName("Temperature_Task");

	temperature_task_tmr.setTaskID("DHT22");
	temperature_task_tmr.setDeadline(0.5);
	temperature_task_tmr.setPeriod(0.5);

	while (1)
	{
		temperature_task_tmr.recordStartTime();
		temperature_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		readDHT22SensorData();

		//Task content ends here -------------------------------------------------

		temperature_task_tmr.recordEndTime();
		temperature_task_tmr.calculateExecutionTime();
		temperature_task_tmr.calculateDeadlineMissPercentage();
		temperature_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&temperature_task_ti_l);
			temperature_task_ti.deadline = temperature_task_tmr.getDeadline();
			temperature_task_ti.deadline_miss_percentage = temperature_task_tmr.getDeadlineMissPercentage();
			temperature_task_ti.execution_time = temperature_task_tmr.getExecutionTime();
			temperature_task_ti.period = temperature_task_tmr.getPeriod();
			temperature_task_ti.prev_slack_time = temperature_task_tmr.getPrevSlackTime();
			temperature_task_ti.task_id = temperature_task_tmr.getTaskID();
			temperature_task_ti.start_time = temperature_task_tmr.getStartTime();
			temperature_task_ti.end_time = temperature_task_tmr.getEndTime();
		pthread_mutex_unlock(&temperature_task_ti_l);
		temperature_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
