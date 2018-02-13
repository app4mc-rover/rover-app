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

rover::RoverGY521::RoverGY521()
:ROVERGY521_SETUP_(0),
i2CAddress(MPU6050_I2C_ADDRESS)
{

}

rover::RoverGY521::RoverGY521(const int custom_i2c_address)
: ROVERGY521_SETUP_(0),
  i2CAddress(custom_i2c_address)
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

	this->ROVERGY521_SETUP_ = 1;
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
	return 0.0;
}



