/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverGrooveUltrasonic API - Interfaces for Rover Groove ultrasonic sensor application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverGrooveUltrasonic class 04.12.2017
 *
 */

#include <roverapi/rover_grooveultrasonic.hpp>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

rover::RoverGrooveUltrasonic::RoverGrooveUltrasonic (const RoverSensorID_t sensor_id)
{
	this->sensorID = sensor_id;
	if (this->sensorID == ROVER_FRONT)
	{
		this->sigPin = SIG1;
	}
	else if (this->sensorID == ROVER_REAR)
	{
		this->sigPin = SIG0;
	}
	else
	{
		printf ("Invalid Sensor ID for the Groove Ultrasonic Sensor to constructor RoverGrooveUltrasonic::RoverGrooveUltrasonic().\n");
		// Adding a default value for the pin in case there is an error
		this->sigPin = SIG1;
	}
}

rover::RoverGrooveUltrasonic::~RoverGrooveUltrasonic(){}

void rover::RoverGrooveUltrasonic::initialize (void)
{

}

float rover::RoverGrooveUltrasonic::read (void)
{
	long startTime, stopTime, elapsedTime, distance = 0;
	pinMode(this->sigPin, OUTPUT);
	digitalWrite(this->sigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(this->sigPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(this->sigPin, LOW);
	pinMode(this->sigPin,INPUT);

	startTime = micros();
	while (digitalRead(this->sigPin) == LOW  );
	startTime = micros();
	// For values above 40cm, groove sensor is unable to receive signals which causes it to stuck
	// This is resolved by adding the timeout below.
	// However, this timeout cause values bigger than 40 to fluctuate
	while (digitalRead(this->sigPin) == HIGH && micros() < startTime + 100000);
	stopTime = micros();
	elapsedTime = stopTime - startTime;
	distance = elapsedTime / 29 /2;
	// The below protection is to ensure there is no value fluctuation
	if (distance > 40 )
		distance = 40;

	return ((float)distance*1.0);
}

void rover::RoverGrooveUltrasonic::setSigPin (const int sig_pin)
{
	this->sigPin = sig_pin;
}

int rover::RoverGrooveUltrasonic::getSigPin (void)
{
	return this->sigPin;
}



