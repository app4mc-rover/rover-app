/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverGY521 API - Interfaces for Rover GY521 accelerometer application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverGY521 class 04.12.2017
 *
 * Disclaimer:
 *    Adapted from https://github.com/OmarAflak/GY-521-Raspberry-Pi-C-/
 */

#include <roverapi/rover_gy521.hpp>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cmath>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define MPU6050_GYRO_XOUT_H        0x43   // R
#define MPU6050_GYRO_YOUT_H        0x45   // R
#define MPU6050_GYRO_ZOUT_H        0x47   // R

#define MPU6050_ACCEL_XOUT_H       0x3B   // R
#define MPU6050_ACCEL_YOUT_H       0x3D   // R
#define MPU6050_ACCEL_ZOUT_H       0x3F   // R

#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_I2C_ADDRESS        0x68   // I2C

static int gy521_fd = -1;

static int16_t xMinRaw_gy = 0;
static int16_t xMaxRaw_gy = 0;
static int16_t yMaxRaw_gy = 0;
static int16_t yMinRaw_gy = 0;

rover::RoverGY521::RoverGY521()
:ROVERGY521_SETUP_(0),
i2CAddress(MPU6050_I2C_ADDRESS),
CALIBRATION_DURATION(10000), 	//compass calibration has a duration of 5 seconds
DECLINATION_ANGLE(0.0413),  	//correction factor for location Paderborn
calibration_start(0)
{

}

rover::RoverGY521::RoverGY521(const int custom_i2c_address)
: ROVERGY521_SETUP_(0),
  i2CAddress(custom_i2c_address),
  CALIBRATION_DURATION(10000), 	//compass calibration has a duration of 5 seconds
  DECLINATION_ANGLE(0.0413),  	//correction factor for location Paderborn
  calibration_start(0)
{
}

rover::RoverGY521::~RoverGY521(){}

void rover::RoverGY521::initialize (void)
{
#if !SIMULATOR
	gy521_fd = wiringPiI2CSetup (this->i2CAddress);
	if (gy521_fd == -1)
	{
		fprintf (stderr, "Unable to initialize GY521 sensor!\n");
	}

	wiringPiI2CReadReg8 (gy521_fd, MPU6050_PWR_MGMT_1);
	wiringPiI2CWriteReg16(gy521_fd, MPU6050_PWR_MGMT_1, 0);
#endif

	this->calibration_start = millis();

	this->ROVERGY521_SETUP_ = 1;
}

void rover::RoverGY521::calibrate (void)
{
	if (this->ROVERGY521_SETUP_)
	{
		int i = 0;
		printf ("Calibrating the GY521 Sensor.. Please turn the rover 360degrees during the next 5 seconds..");
		this->calibration_start = millis();

		for (i = 0; i<40; i++)
		{
			printf ("GY521 Sensor Calibration: %f second left\n", (((40-i)*125.0)/1000)*1.0);
			this->read();
			delay(125);
		}
		printf ("GY521 Sensor Calibration: Done\n");
	}
	else
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
}

int8_t rover::RoverGY521::getGyroX()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_GYRO_XOUT_H);
	}
#endif
}

int8_t rover::RoverGY521::getGyroY()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_GYRO_YOUT_H);
	}
#endif
}

int8_t rover::RoverGY521::getGyroZ()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_GYRO_ZOUT_H);
	}
#endif
}

int8_t rover::RoverGY521::getAccelX()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_ACCEL_XOUT_H);
	}
#endif
}

int8_t rover::RoverGY521::getAccelY()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_ACCEL_YOUT_H);
	}
#endif
}

int8_t rover::RoverGY521::getAccelZ()
{
#if SIMULATOR
	return 10;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		return (int8_t) wiringPiI2CReadReg8(gy521_fd, MPU6050_ACCEL_ZOUT_H);
	}
#endif
}

float rover::RoverGY521::getAngleX()
{
#if SIMULATOR
	return 0.0;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		int x = getAccelX();
		int y = getAccelY();
		int z = getAccelZ();
		float ax = atan(x/(sqrt(y*y+z*z)))* 180/M_PI;
		return ax;
	}
#endif
}

float rover::RoverGY521::getAngleY()
{
#if SIMULATOR
	return 0.0;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		int x = getAccelX();
		int y = getAccelY();
		int z = getAccelZ();
		float ay = atan(y/(sqrt(x*x+z*z)))* 180/M_PI;
		return ay;
	}
#endif
}

float rover::RoverGY521::getAngleZ()
{
#if SIMULATOR
	return 0.0;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		int x = getAccelX();
		int y = getAccelY();
		int z = getAccelZ();
		float az = atan((sqrt(y*y+x*x))/z)* 180/M_PI;
		return az;
	}
#endif
}

float rover::RoverGY521::read (void)
{
#if SIMULATOR
	return 0.0;
#else
	if (this->ROVERGY521_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverGY521. Use RoverGY521::initialize() !\n");
	}
	else
	{
		int xRaw = getAccelX();
		int yRaw = getAccelY();

		//if calibration is active calculate minimum and maximum x/y values for calibration
		if (millis() <= this->calibration_start + this->CALIBRATION_DURATION)
		{
			xMinRaw_gy = MINIMUM_<int16_t>(xRaw, xMinRaw_gy);

			xMaxRaw_gy = MAXIMUM_<int16_t>(xRaw, xMaxRaw_gy);

			yMinRaw_gy = MINIMUM_<int16_t>(yRaw, yMinRaw_gy);

			yMaxRaw_gy = MAXIMUM_<int16_t>(yRaw, yMaxRaw_gy);
		}

		//calibration: move and scale x coordinates based on minimum and maximum values to get a unit circle
		float xf = xRaw - (float) (xMinRaw_gy + xMaxRaw_gy) / 2.0f;
		xf = xf / (xMinRaw_gy + xMaxRaw_gy) * 2.0f;

		//calibration: move and scale y coordinates based on minimum and maximum values to get a unit circle
		float yf = yRaw - (float) (yMinRaw_gy + yMaxRaw_gy) / 2.0f;
		yf = yf / (yMinRaw_gy + yMaxRaw_gy) * 2.0f;

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

template<typename T>
inline T rover::RoverGY521::MINIMUM_(const T& a, const T& b)
{
	return (a < b ? a : b);
}

template<typename T>
inline T rover::RoverGY521::MAXIMUM_(const T& a, const T& b)
{
	return (a > b ? a : b);
}
