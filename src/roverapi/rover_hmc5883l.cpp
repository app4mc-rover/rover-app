/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverHMC5883L API - Interfaces for Rover HMC5883L bearing sensor application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverHMC5883L class 04.12.2017
 *    David Schmelter, Fraunhofer IEM - compass sensor initial implementation
 *
 */

#include <roverapi/rover_hmc5883l.hpp>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


rover::RoverHMC5883L::RoverHMC5883L()
:HMC588L_ADDRESS(0x1E),			//default: 0x1E
 CALIBRATION_DURATION(10000), 	//compass calibration has a duration of 5 seconds
 DECLINATION_ANGLE(0.0413),  	//correction factor for location Paderborn
 calibration_start(0),
 ROVERHMC5883L_SETUP_(0),
 xMinRaw(0),
 xMaxRaw(0),
 yMinRaw(0),
 yMaxRaw(0),
 i2c_hmc588l_fd(-1)
{

}

rover::RoverHMC5883L::~RoverHMC5883L(){}

void rover::RoverHMC5883L::initialize (void)
{
#ifdef DEBUG
	printf ("HMC588L Address is: %x\n", this->HMC588L_ADDRESS);
	printf ("HMC588L Calibration period is %d\n", this->CALIBRATION_DURATION);
	printf ("HMC588L Declination angle is %f\n", this->DECLINATION_ANGLE);
#endif

#if !SIMULATOR
	if ((i2c_hmc588l_fd = wiringPiI2CSetup(this->HMC588L_ADDRESS)) < 0) {
		printf("Failed to initialize HMC588L compass sensor");
	}

	if (i2c_hmc588l_fd >= 0) {
		int8_t gain = 5;

		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x00, 0x70); // 8-average, 15 Hz default, positive self test measurement
		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x01, gain << 5); // Gain
		wiringPiI2CWriteReg8(i2c_hmc588l_fd, 0x02, 0x00); // Continuous-measurement mode
	}

	this->calibration_start = millis();

#endif

	this->ROVERHMC5883L_SETUP_ = 1;


}

void rover::RoverHMC5883L::calibrate (void)
{
	this->calibration_start = millis();
}

float rover::RoverHMC5883L::read (void)
{
#if SIMULATOR
	return 0.5f;
#else
	if (this->ROVERHMC5883L_SETUP_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverHMC5883L. Use RoverHMC5883L()::initialize() !\n");
	}
	else
	{
		int8_t buffer[6];

		//potential optimization: wiringPiI2CReadReg16
		buffer[0] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x03); //MSB x
		buffer[1] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x04); //LSB x
		buffer[2] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x05); //MSB z
		buffer[3] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x06); //LSB z
		buffer[4] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x07); //MSB y
		buffer[5] = wiringPiI2CReadReg8(i2c_hmc588l_fd, 0x08); //LSB y

		int16_t xRaw = (((int16_t) buffer[0] << 8) & 0xff00) | buffer[1];
		//int16_t zRaw = (((int16_t) buffer[2] << 8) & 0xff00) | buffer[3];
		int16_t yRaw = (((int16_t) buffer[4] << 8) & 0xff00) | buffer[5];

		//if calibration is active calculate minimum and maximum x/y values for calibration
		if (millis() <= this->calibration_start + this->CALIBRATION_DURATION)
		{
			xMinRaw = MINIMUM_(xRaw, xMinRaw);

			xMaxRaw = MAXIMUM_(xRaw, xMaxRaw);

			yMinRaw = MINIMUM_(yRaw, yMinRaw);

			yMaxRaw = MAXIMUM_(yRaw, yMaxRaw);
		}

		//calibration: move and scale x coordinates based on minimum and maximum values to get a unit circle
		float xf = xRaw - (float) (xMinRaw + xMaxRaw) / 2.0f;
		xf = xf / (xMinRaw + xMaxRaw) * 2.0f;

		//calibration: move and scale y coordinates based on minimum and maximum values to get a unit circle
		float yf = yRaw - (float) (yMinRaw + yMaxRaw) / 2.0f;
		yf = yf / (yMinRaw + yMaxRaw) * 2.0f;

		float bearing = atan2(yf, xf);
	#ifdef DEBUG
		printf("%f, bearing\n", bearing);
	#endif

		//location specific magnetic field correction
		bearing += this->DECLINATION_ANGLE;

		if (bearing < 0) {
			bearing += 2 * M_PI;
		}

		if (bearing > 2 * M_PI) {
			bearing -= 2 * M_PI;
		}

		float headingDegrees = bearing * (180.0 / M_PI);
	#ifdef DEBUG
		printf("%lf, headingDegrees\n", headingDegrees);
	#endif
		return headingDegrees;
	}
#endif
}

void rover::RoverHMC5883L::setHMC588LAddress (const int address)
{
	this->HMC588L_ADDRESS = address;
}

void rover::RoverHMC5883L::setHMC588LDeclinationAngle (const float angle)
{
	this->DECLINATION_ANGLE = angle;
}

void rover::RoverHMC5883L::setHMC588LCalibrationPeriod(const int period)
{
	this->CALIBRATION_DURATION = period;
}

int rover::RoverHMC5883L::getHMC588LAddress (void)
{
	return this->HMC588L_ADDRESS;
}

float rover::RoverHMC5883L::getHMC588LDeclinationAngle (void)
{
	return this->DECLINATION_ANGLE;
}

int rover::RoverHMC5883L::getHMC588LCalibrationPeriod (void)
{
	return this->CALIBRATION_DURATION;
}

template<typename T>
inline T rover::RoverHMC5883L::MINIMUM_(const T& a, const T& b)
{
	return (a < b ? a : b);
}

template<typename T>
inline T rover::RoverHMC5883L::MAXIMUM_(const T& a, const T& b)
{
	return (a > b ? a : b);
}
