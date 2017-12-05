/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverGY521 API - Interfaces for Rover GY521 accelerometer application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverGY521 class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_GY521_HPP_
#define ROVERAPI_ROVER_GY521_HPP_

#include <roverapi/rover_sensor.hpp>

namespace rover
{
	/**
	 * @brief RoverGY521 is a class that is inherited from RoverSensor abstract class. RoverGY521 class contains member functions and variables to set up and read from GY521 accelerometer that is embedded on the rover.
	 */
	class RoverGY521 : public RoverSensor
	{
		private:

		public:
			/**
			 * @brief Constructor for the RoverGY521 class
			 */
			explicit RoverGY521 ();

			/**
			 * @brief Destructor for the RoverGY521 class
			 */
			~RoverGY521();

			/**
			 * @brief Function to setup RoverGY521 sensor
			 * @return void
			 */
			void initialize (void);

			float read (void);

	};
}




#endif /* ROVERAPI_ROVER_GY521_HPP_ */
