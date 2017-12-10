/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverHCSR04 API - Interfaces for Rover ultrasonic sensor application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverHCSR04 class 04.12.2017
 *
 */

#include <roverapi/rover_hcsr04.hpp>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

rover::RoverHCSR04::RoverHCSR04 (const RoverSensorID_t sensor_id)
:ROVERHCSR04_SETUP_(0)
{
	this->sensorID = sensor_id;
	if (this->sensorID == ROVER_FRONT)
	{
		this->trigPin = TRIG1;
		this->echoPin = ECHO1;
	}
	else if (this->sensorID == ROVER_REAR)
	{
		this->trigPin = TRIG0;
		this->echoPin = ECHO0;
	}
	else
	{
		printf ("Invalid Sensor ID for the Ultrasonic Sensor to constructor RoverHCSR04::RoverHCSR04().\n");
		// Adding a default value for the pins in case there is an error
		this->trigPin = TRIG0;
		this->echoPin = ECHO0;
	}
}

rover::RoverHCSR04::~RoverHCSR04(){}

void rover::RoverHCSR04::initialize (void)
{
#if !SIMULATOR
    pinMode(this->trigPin, OUTPUT);
    pinMode(this->echoPin, INPUT);

    //TRIG pin must start LOW
    digitalWrite(this->trigPin, LOW);
    delayMicroseconds(2);
#endif

    this->ROVERHCSR04_SETUP_ = 1;
}

float rover::RoverHCSR04::read (void)
{
#if SIMULATOR
	return 0.5f;
#else
	if (this->ROVERHCSR04_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverHCSR04. Use RoverHCSR04()::initialize() !\n");
	}
	else
	{
		int distance = 0;
		//Send trig pulse
		digitalWrite(this->trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(this->trigPin, LOW);

		//Wait for echo start
		long startTime = micros();
		while(digitalRead(this->echoPin) == LOW && micros() < startTime + 100000);

		//Wait for echo end
		startTime = micros();
		while(digitalRead(this->echoPin) == HIGH);
		long travelTime = micros() - startTime;

		//Get distance in cm
		distance = travelTime * 34300;
		distance = distance / 1000000;
		distance = distance / 2;
		// The below protection is to ensure there is no value fluctuation due to timeout
		if (distance > 40 )
			distance = 40;

		//	printf("dist=%d\n",distance);
		return ((float) distance*1.0);
	}
#endif
}

void rover::RoverHCSR04::setTrigPin (const int trig_pin)
{
	this->trigPin = trig_pin;
}

int rover::RoverHCSR04::getTrigPin (void)
{
	return this->trigPin;
}

void rover::RoverHCSR04::setEchoPin (const int echo_pin)
{
	this->echoPin = echo_pin;
}

int rover::RoverHCSR04::getEchoPin (void)
{
	return this->echoPin;
}

