/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Sensor API - Interfaces for Rover sensors application development
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, modified C++ API 04.12.2017
 *    										created C++ API 17.11.2017
 *    										initial QMC5883L driver implemented 30.11.2017
 *    David Schmelter, Fraunhofer IEM - compass sensor initial implementation
 *    Gael Blondelle, Eclipse Foundation - initial API and parameters
 *
 */

#ifndef API_ROVER_SENSORS_HPP_
#define API_ROVER_SENSORS_HPP_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


//To debug the application, uncomment the following
//#define DEBUG 1


namespace rover
{
	/**
	 * @brief RoverSensorID_t defines the sensor location ID of the proximity sensors on the rover
	 */
	enum RoverSensorID_t {	ROVER_REAR_RIGHT = 0,
							ROVER_REAR_LEFT = 1,
							ROVER_FRONT_RIGHT = 2,
							ROVER_FRONT_LEFT = 3,
							ROVER_FRONT = 4,
							ROVER_REAR = 5};

	/**
	 * @brief RoverSensor class is an abstract class to define interface between common sensors used in the rover.
	 */
	class RoverSensor
	{
		public:
			/**
			 * @brief Virtual function (interface) for setting up sensors
			 * @return void
			 */
			virtual void initialize (void) = 0;

			/**
			 * @brief Virtual function (interface) for reading from sensors
			 * @return sensor_val Sensor value read from sensor
			 */
			virtual float read (void) = 0;

			/**
			 * @brief Destructor for the RoverSensor class
			 */
			virtual ~RoverSensor() {};

	};
}



#endif /* API_ROVER_SENSORS_HPP_ */
