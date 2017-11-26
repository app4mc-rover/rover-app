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
\mainpage Rover API Main Page

\section api_info roverapp and Rover API Info

Rover software, called **roverapp** features a multi-threaded (POSIX threads or Pthreads) C/C++ implementation that runs on Linux-based embedded single board computers (such as Raspberry Pi).
Rover features countless threads dedicated to communication infrastructure, sensor driving, display unit (such as OLED displays) utilization, bluetooth communication, image processing, and behavior modes (such as Parking, Adaptive Cruise Control, Manual Driving, and Booth Modes).
It also features drivers for sensors such as magnetometers, accelerometers, various ultrasonic sensors, and camera modules. Furthermore, OLED display, buttons, a buzzer are utilized.

Roverapp builds and contains the **Rover API**, which is able to handle subset of its functionality. Example functionality covered in **Rover API** is given below:

\li **RoverBase** class is the main class used for instantiating rover objects.
Each RoverBase object has private member functions containing other API classes in singleton fashion.
Therefore, using one RoverBase class, other classes can be accessed, and operations such as initialization,
shutting down could be performed.
\li **RoverCloud** contains the member functions to connect and send data to Eclipse Hono instance using several parameters such as host name, port, tenant name, user name, and password. This class wraps hono_interaction library for Rover-specific interactions.
\li **RoverDisplay** contains the member functions to control OLED display on the Rover. This class is a wrapper API for Adafruit_GFX and Adafruit_SSD1306 libraries.
\li **RoverDriving** contains the member functions to drive the rover using its motors.
\li **RoverGpio** class provides the member functions related to embedded buzzer, user button, and shutdown button on the rover.
\li **RoverSensors** class contains member functions to setup and read from embedded sensors from rover's RoverSenseLayer sensor layer.
\li **RoverUtils** contains member functions to retrieve connectivity status and core utilization of the rover. This class wraps status_library lib, developed for the rover-app.

\image html ./images/rover2.jpg

\section example_usage Rover API Example Usage
Some Info here

\section roverdocs Rover Documentation

Link: <a href="http://app4mc-rover.github.io/rover-docs">Rover Complete Documentation</a>

\copyright Copyright(C) 2017 IDiAL Institute, Fachhochschule Dortmund

*/

/**
  *   @brief  rover Namespace contains classes to manage Rover sensors, gpio, driving, utilities, and cloud.
  */
namespace rover
{
	/**
	  *   @brief  RoverBase class is the main class used for instantiating rover objects.
	  *   Each RoverBase object has private member functions containing other API classes in singleton fashion.
	  *   Therefore, using one RoverBase class, other classes can be accessed, and operations such as initialization,
	  *   shutting down could be performed.
	  */
	class RoverBase
	{
		private:
			/**
			 * @brief Private RoverCloud instance that is accessed from RoverBase
			 */
			RoverCloud myRoverCloud;

			/**
			 * @brief Private RoverDisplay instance that is accessed from RoverBase
			 */
			RoverDisplay myRoverDisplay;

			/**
			 * @brief Private RoverDriving instance that is accessed from RoverBase
			 */
			RoverDriving myRoverDriving;

			/**
			 * @brief Private RoverGpio instance that is accessed from RoverBase
			 */
			RoverGpio myRoverGpio;

			/**
			 * @brief Private RoverSensors instance that is accessed from RoverBase
			 */
			RoverSensors myRoverSensors;

			/**
			 * @brief Private RoverUtils instance that is accessed from RoverBase
			 */
			RoverUtils myRoverUtils;

		public:
			/**
			 * @brief Constructor for the RoverBase class
			 */
			RoverBase();

			/**
			 * @brief Destructor for the RoverBase class
			 */
			virtual ~RoverBase();

			/**
			 * @brief Initializes the RoverBase
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Shuts down the Rover's OS.
			 * @return void
			 */
			void shutdown (void);

			/**
			 * @brief Sleep function to be used in rover applications
			 * @param Period to sleep in milliseconds
			 * @return void
			 */
			void sleep (unsigned int period_ms);

			/**
			 * @brief Public function to access private RoverCloud instance.
			 * @return RoverCloud instance
			 */
			rover::RoverCloud inRoverCloud (void);

			/**
			 * @brief Public function to access private RoverDisplay instance.
			 * @return RoverDisplay instance
			 */
			rover::RoverDisplay inRoverDisplay (void);

			/**
			 * @brief Public function to access private RoverDriving instance.
			 * @return RoverDriving instance
			 */
			rover::RoverDriving inRoverDriving (void);

			/**
			 * @brief Public function to access private RoverGpio instance.
			 * @return RoverGpio instance
			 */
			rover::RoverGpio inRoverGpio (void);

			/**
			 * @brief Public function to access private RoverSensors instance.
			 * @return RoverSensors instance
			 */
			rover::RoverSensors inRoverSensors (void);

			/**
			 * @brief Public function to access private RoverUtils instance.
			 * @return RoverUtils instance
			 */
			rover::RoverUtils inRoverUtils (void);
	};
}



#endif /* ROVER_API_HPP_ */
