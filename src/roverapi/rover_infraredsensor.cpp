/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverInfraredSensor API - Interfaces for Rover infrared sensor application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverInfraredSensor class 04.12.2017
 *    Gael Blondelle, Eclipse Foundation, initial C-API and parameters
 */

#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/basic_psys_rover.h>
#include <mcp3004.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

rover::RoverInfraredSensor::RoverInfraredSensor (const RoverSensorID_t sensor_id)
:ROVERINFRAREDSENSOR_SETUP_(0)
{
	if (sensor_id != ROVER_FRONT_LEFT &&
		sensor_id != ROVER_FRONT_RIGHT &&
		sensor_id != ROVER_REAR_LEFT &&
		sensor_id != ROVER_REAR_RIGHT)
	{
		printf ("Invalid Sensor ID for the Infrared Sensor to constructor RoverInfraredSensor::RoverInfraredSensor().\n");
		this->sensorID = ROVER_FRONT_LEFT;
	}
	else
	{
		this->sensorID = sensor_id;
	}
}

rover::RoverInfraredSensor::~RoverInfraredSensor(){}

void rover::RoverInfraredSensor::initialize (void)
{
	/* mcp3004Setup can only be called once per program, One solution: */
	static class Once2 { public: Once2(){
		/* Init the analog digital converter */
		mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels
	}} Once2_;

	this->ROVERINFRAREDSENSOR_SETUP_ = 1;
}

float rover::RoverInfraredSensor::read (void)
{
	if (this->ROVERINFRAREDSENSOR_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverInraredSensor. Use RoverInfraredSensor()::initialize() !\n");
	}
	else
	{
		float x;
		float y = analogRead (BASE+this->sensorID);

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
}


