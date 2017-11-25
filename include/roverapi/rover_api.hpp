/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover API - Interfaces for Rover application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef ROVER_API_HPP_
#define ROVER_API_HPP_

#include <stdint.h>

#include <roverapi/rover_display.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_gpio.hpp>
#include <roverapi/rover_sensors.hpp>
#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_cloud.hpp>

/**
\mainpage Rover API Main Page - Example Usage of Rover API

This is a simple example of a mainpage you can create yourself.
Place this inside of a file called `mainpage.dox` and use doxygen.
If you specified `INPUT` or `FILE_PATTERNS` in your Doxyfile please
add `.dox` to your file patterns or `mainpage.dox` to your INPUT files.
*/

/**
  *   @brief  rover Namespace contains classes to manage Rover sensors, gpio, driving, utilities, and cloud.
  */
namespace rover
{
	class RoverBase
	{
		private:
			RoverCloud myRoverCloud;
			RoverDisplay myRoverDisplay;
			RoverDriving myRoverDriving;
			RoverGpio myRoverGpio;
			RoverSensors myRoverSensors;
			RoverUtils myRoverUtils;

		public:
			RoverBase();
			virtual ~RoverBase();
			void initialize (void);
			void start (void);
			void sleep (int period_ms);
			void kill (void);

			rover::RoverCloud inRoverCloud (void);
			rover::RoverDisplay inRoverDisplay (void);
			rover::RoverDriving inRoverDriving (void);
			rover::RoverGpio inRoverGpio (void);
			rover::RoverSensors inRoverSensors (void);
			rover::RoverUtils inRoverUtils (void);
	};
}



#endif /* ROVER_API_HPP_ */
