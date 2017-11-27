/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Sensors API - Interfaces for Rover sensors application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++ API 17.11.2017
 *    Gael Blondelle - initial API and parameters
 *
 */

#include <roverapi/basic_psys_rover.h>
#include <roverapi/rover_sensors.hpp>
#include <mcp3004.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static int i2c_hmc588l_fd = -1;

static int16_t xMinRaw = 0;
static int16_t xMaxRaw = 0;
static int16_t yMaxRaw = 0;
static int16_t yMinRaw = 0;

rover::RoverSensors::RoverSensors()
{
	this->calibration_start = 0;
}

void rover::RoverSensors::initialize (void)
{
	//wiringPiSetup();

	this->setupHCSR04UltrasonicSensor(this->ROVER_FRONT);
	this->setupHCSR04UltrasonicSensor(this->ROVER_REAR);
	this->setupInfraredSensors();
	this->setupBearingSensor();
}

void rover::RoverSensors::setupHCSR04UltrasonicSensor (int sensor_id)
{
	int trig_pin, echo_pin;

	if (sensor_id == 0)
	{
		trig_pin = this->TRIG1;
		echo_pin = this->ECHO1;
	}
	else
	{
		trig_pin = this->TRIG0;
		echo_pin = this->ECHO0;
	}

    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);

    //TRIG pin must start LOW
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
}

int rover::RoverSensors::readHCSR04UltrasonicSensor (int sensor_id)
{
	int trig_pin, echo_pin;

	if (sensor_id == 0)
	{
		trig_pin = this->TRIG1;
		echo_pin = this->ECHO1;
	}
	else
	{
		trig_pin = this->TRIG0;
		echo_pin = this->ECHO0;
	}

	int distance = 0;
    //Send trig pulse
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    //Wait for echo start
    long startTime = micros();
    while(digitalRead(echo_pin) == LOW && micros() < startTime + 100000);

    //Wait for echo end
    startTime = micros();
    while(digitalRead(echo_pin) == HIGH);
    long travelTime = micros() - startTime;

    //Get distance in cm
    distance = travelTime * 34300;
	distance = distance / 1000000;
	distance = distance / 2;
	// The below protection is to ensure there is no value fluctuation due to timeout
	if (distance > 40 )
		distance = 40;

	//	printf("dist=%d\n",distance);
    return distance;
}

void rover::RoverSensors::setupGrooveUltrasonicSensor(void) {
	//wiringPiSetup();   //Since this can only be used once in a program, we do it in main and comment this.
}

int rover::RoverSensors::readGrooveUltrasonicSensor (int sensor_id)
{
	int sig_pin;

	if (sensor_id == 0)
	{
		sig_pin = this->SIG1;
	}
	else
	{
		sig_pin = this->SIG0;
	}

	long startTime, stopTime, elapsedTime, distance = 0;
	pinMode(sig_pin, OUTPUT);
	digitalWrite(sig_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(sig_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(sig_pin, LOW);
	pinMode(sig_pin,INPUT);

	startTime = micros();
	while (digitalRead(sig_pin) == LOW  );
	startTime = micros();
	// For values above 40cm, groove sensor is unable to receive signals which causes it to stuck
	// This is resolved by adding the timeout below.
	// However, this timeout cause values bigger than 40 to fluctuate
	while (digitalRead(sig_pin) == HIGH && micros() < startTime + 100000);
	stopTime = micros();
	elapsedTime = stopTime - startTime;
	distance = elapsedTime / 29 /2;
	// The below protection is to ensure there is no value fluctuation
	if (distance > 40 )
		distance = 40;
	return distance;
}

void rover::RoverSensors::setupInfraredSensors (void)
{
	/* Init the analog digital converter */
	mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels
}

float rover::RoverSensors::readInfraredSensor (int infrared_sensor_id)
{
	float x;
	float y = analogRead (BASE+infrared_sensor_id);

	// 1/cm to output voltage is almost linear between
	// 80cm->0,4V->123
	// 6cm->3,1V->961
	// => y=5477*x+55 => x= (y-55)/5477
	if (y<123){
		x=100.00;
	} else {
		float inverse = (y-55)/5477;
		//printf("inverse=%f\n",inverse);
	// x is the distance in cm
		x = 1/inverse;
	}

	return x;
}

void rover::RoverSensors::calibrateBearingSensor (void)
{
	this->calibration_start = millis();
}

void rover::RoverSensors::setupBearingSensor(void)
{
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
}

float rover::RoverSensors::readBearing(void)
{
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
	if (millis() <= this->calibration_start + this->CALIBRATION_DURATION) {
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
	bearing += this->DECLINATION_ANGLE;

	if (bearing < 0) {
		bearing += 2 * M_PI;
	}

	if (bearing > 2 * M_PI) {
		bearing -= 2 * M_PI;
	}

	float headingDegrees = bearing * (180.0 / M_PI);

	return headingDegrees;
}

float rover::RoverSensors::readTemperature (void)
{
	int data[5] = { 0, 0, 0, 0, 0 };

	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	int try_again = 1;
	float f, h, c;

	while (try_again == 1)
	{

		/* pull pin down for 18 milliseconds */
		pinMode( this->DHT22_RPI_PIN, OUTPUT );
		digitalWrite( this->DHT22_RPI_PIN, LOW );
		delay( 18 );

		/* prepare to read the pin */
		pinMode( this->DHT22_RPI_PIN, INPUT );

		/* detect change and read data */
		for ( i = 0; i < this->MAX_TIMINGS; i++ )
		{
			counter = 0;
			while ( digitalRead( this->DHT22_RPI_PIN ) == laststate )
			{
				counter++;
				delayMicroseconds( 1 );
				if ( counter == 255 )
				{
					break;
				}
			}
			laststate = digitalRead( this->DHT22_RPI_PIN );

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
			h = (float)((data[0] << 8) + data[1]) / 10;
			if ( h > 100 )
			{
				h = data[0];	// for DHT11
			}
			c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
			if ( c > 125 )
			{
				c = data[2];	// for DHT11
			}
			if ( data[2] & 0x80 )
			{
				c = -c;
			}
			f = c * 1.8f + 32;
			//printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
			try_again = 0;
		}
		else
		{
			/* Data not good */
			try_again = 1;
		}
	}

	/* Return temperature */
	return c;
}

float rover::RoverSensors::readHumidity (void)
{
	int data[5] = { 0, 0, 0, 0, 0 };

	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	int try_again = 1;
	float f, h, c;

	while (try_again == 1)
	{

		/* pull pin down for 18 milliseconds */
		pinMode( this->DHT22_RPI_PIN, OUTPUT );
		digitalWrite( this->DHT22_RPI_PIN, LOW );
		delay( 18 );

		/* prepare to read the pin */
		pinMode( this->DHT22_RPI_PIN, INPUT );

		/* detect change and read data */
		for ( i = 0; i < this->MAX_TIMINGS; i++ )
		{
			counter = 0;
			while ( digitalRead( this->DHT22_RPI_PIN ) == laststate )
			{
				counter++;
				delayMicroseconds( 1 );
				if ( counter == 255 )
				{
					break;
				}
			}
			laststate = digitalRead( this->DHT22_RPI_PIN );

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
			h = (float)((data[0] << 8) + data[1]) / 10;
			if ( h > 100 )
			{
				h = data[0];	// for DHT11
			}
			c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
			if ( c > 125 )
			{
				c = data[2];	// for DHT11
			}
			if ( data[2] & 0x80 )
			{
				c = -c;
			}
			f = c * 1.8f + 32;
			//printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
			try_again = 0;
		}
		else
		{
			/* Data not good */
			try_again = 1;
		}
	}

	/* Return humidity */
	return h;
}
