/*
 * Copyright (c) 2017 Fraunhofer IEM, Eclipse Foundation and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    HMC588L compass reading and calibration task (x,y plane) with with wiringPi and pThreads
 *
 * Authors:
 *     David Schmelter
 *     <david.schmelter@iem.fraunhofer.de>
 *
 * Contributors:
 * 		Mustafa Ozcelikors <mozcelikors@gmail.com> Calibration via web-page, changes regarding schedulability
 *
 * Update History:
 *    22.03.2017 - initial revision
 *
 */
#include <tasks/compass_sensor_task.h>
#include <stdint.h>
#include <ctime>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>
#include <math.h>

#include <fstream>
#include <iostream>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>

#define HMC588L_ADDRESS 0x1E
#define CALIBRATION_DURATION 10000 //compass calibration has a duration of 5 seconds
#define DECLINATION_ANGLE 0.0413 //correction factor for location Paderborn

static int i2c_hmc588l_fd = -1;
static unsigned int calibration_start = 0;

static int16_t xMinRaw = 0;
static int16_t xMaxRaw = 0;
static int16_t yMaxRaw = 0;
static int16_t yMinRaw = 0;

void setupCompassSensor() {

	if ((i2c_hmc588l_fd = wiringPiI2CSetup(HMC588L_ADDRESS)) < 0) {
		printf("Failed to initialize HMC588L compass sensor");
	}

	if (i2c_hmc588l_fd >= 0) {
		int8_t gain = 5;

		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x00, 0x70); // 8-average, 15 Hz default, positive self test measurement
		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x01, gain << 5); // Gain
		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x02, 0x00); // Continuous-measurement mode
	}

	calibration_start = millis();
}

float getBearingFromSensor() {

	int8_t buffer[6];
	//potential optimization: wiringPiI2CReadReg16
	buffer[0] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x03);
	buffer[1] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x04);
	buffer[2] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x05);
	buffer[3] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x06);
	buffer[4] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x07);
	buffer[5] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x08);

	int16_t xRaw = (((int16_t) buffer[0] << 8) & 0xff00) | buffer[1];
	//int16_t zRaw = (((int16_t) buffer[2] << 8) & 0xff00) | buffer[3];
	int16_t yRaw = (((int16_t) buffer[4] << 8) & 0xff00) | buffer[5];

	//if calibration is active calculate minimum and maximum x/y values for calibration
	if (millis() <= calibration_start + CALIBRATION_DURATION) {
		if (xRaw < xMinRaw) {
			xMinRaw = xRaw;
		}
		if (xRaw > xMaxRaw) {
			xMaxRaw = xRaw;
		}
		if (yRaw < yMinRaw) {
			yMinRaw = yRaw;
		}
		if (yRaw > yMaxRaw) {
			yMaxRaw = yRaw;
		}
	}

	//calibration: move and scale x coordinates based on minimum and maximum values to get a unit circle
	float xf = xRaw - (float) (xMinRaw + xMaxRaw) / 2.0f;
	xf = xf / (xMinRaw + xMaxRaw) * 2.0f;

	//calibration: move and scale y coordinates based on minimum and maximum values to get a unit circle
	float yf = yRaw - (float) (yMinRaw + yMaxRaw) / 2.0f;
	yf = yf / (yMinRaw + yMaxRaw) * 2.0f;

	float bearing = atan2(yf, xf);

	//location specific magnetic field correction
	bearing += DECLINATION_ANGLE;

	if (bearing < 0) {
		bearing += 2 * M_PI;
	}

	if (bearing > 2 * M_PI) {
		bearing -= 2 * M_PI;
	}

	float headingDegrees = bearing * (180.0 / M_PI);

	return headingDegrees;
}

int EndCalibrationMode (void)
{
	//Write to text file ..
	ofstream myfile;
	myfile.open("/var/www/html/ROVER_CMD.inc");
	myfile << "F";
	myfile.close();
	pthread_mutex_lock(&keycommand_lock);
	keycommand_shared = 'f';
	pthread_mutex_unlock(&keycommand_lock);
	return 1;
}

void *CompassSensor_Task(void * arg)
{
	timing compass_task_tmr;
	char local_command = 'f';

	CollectThreadName("CompassSensor_Task");

	compass_task_tmr.setTaskID("Compass-Sensor");
	compass_task_tmr.setDeadline(0.1);
	compass_task_tmr.setPeriod(0.1);

	setupCompassSensor();

	while (1) {
		compass_task_tmr.recordStartTime();
		compass_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		local_command = keycommand_shared;

		//Calibration routine
		if (local_command == 'u')
		{
			printf("Starting compass calibration for 5 seconds. Please rotate me 360 degrees.\n");
			EndCalibrationMode();
			calibration_start = millis();
			//Calibration state..
			//At the end of calibration to go to the end of calibration mode call EndCalibrationMode()
		}
		//Asynchronous end to calibration mode --> Call EndCalibrationMode()

		if (i2c_hmc588l_fd >= 0) {
			pthread_mutex_lock(&compass_lock);
				bearing_shared = getBearingFromSensor();
			pthread_mutex_unlock(&compass_lock);
			//printf("Bearing=%f\n", bearing_shared);
		}

		//Task content ends here -------------------------------------------------
		compass_task_tmr.recordEndTime();
		compass_task_tmr.calculateExecutionTime();
		compass_task_tmr.calculateDeadlineMissPercentage();
		compass_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&compass_task_ti_l);
			compass_task_ti.deadline = compass_task_tmr.getDeadline();
			compass_task_ti.deadline_miss_percentage = compass_task_tmr.getDeadlineMissPercentage();
			compass_task_ti.execution_time = compass_task_tmr.getExecutionTime();
			compass_task_ti.period = compass_task_tmr.getPeriod();
			compass_task_ti.prev_slack_time = compass_task_tmr.getPrevSlackTime();
			compass_task_ti.task_id = compass_task_tmr.getTaskID();
			compass_task_ti.start_time = compass_task_tmr.getStartTime();
			compass_task_ti.end_time = compass_task_tmr.getEndTime();
		pthread_mutex_unlock(&compass_task_ti_l);
		compass_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
