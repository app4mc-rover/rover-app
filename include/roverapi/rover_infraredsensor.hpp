/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    RoverInfraredSensor API - Interfaces for Rover infrared sensor application development
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverInfraredSensor class 04.12.2017
 *    Gael Blondelle, Eclipse Foundation, initial C-API and parameters
 */

#ifndef ROVERAPI_ROVER_INFRAREDSENSOR_HPP_
#define ROVERAPI_ROVER_INFRAREDSENSOR_HPP_

#include <roverapi/rover_sensor.hpp>

namespace rover
{
	/**
	 * @brief RoverInfraredSensor is a class that is inherited from RoverSensor abstract class. RoverInfraredSensor class contains member functions and variables to set up and read from SHARP infrared sensors that are embedded on the rover.
	 */
	class RoverInfraredSensor : public RoverSensor
	{
		private:
			/**
			 * @brief Sensor Identification
			 */
			RoverSensorID_t sensorID;

			/**
			 * @brief Flag to hold if RoverInfraredSensor is set up.
			 */
			int ROVERINFRAREDSENSOR_SETUP_;

		public:

			/**
			 * @brief Constructor for the RoverInfraredSensor class
			 * @param sensor_id Sensor to address
			 */
			explicit RoverInfraredSensor (const RoverSensorID_t sensor_id);

			/**
			 * @brief Destructor for the RoverInfraresSensor class
			 */
			~RoverInfraredSensor();

			/**
			 * @brief Function to setup Analog to Digital converter used in infrared sensors. Should be called once per program.
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Member function to read from infrared proximity sensor given its sensor id.
			 * @return sensor_val Sensor value read in centimeters from sensor
			 */
			float read (void);
	};
}



#endif /* ROVERAPI_ROVER_INFRAREDSENSOR_HPP_ */
