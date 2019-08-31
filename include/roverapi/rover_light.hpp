/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Driving API - Interfaces for Rover driving application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */




/*
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include "dma.h"
#include "pwm.h"
#include "version.h"
#include "ws2811.h"

*/


#include <stdio.h>

#ifndef API_ROVER_LIGHT_HPP_
#define API_ROVER_LIGHT_HPP_

//#include <libraries/lightsys/ws2811.h>
#include <stdio.h>

namespace rover
{
	/* Rover driving speeds */
	
	/**
	 * @brief Static definition to hold lowest driving speed for rover.
	 */
	//static const int LOWEST_SPEED = 360;

	/**
	 * @brief Static definition to hold highest driving speed for rover.
	 */
	//static const int HIGHEST_SPEED = 480;

	/**
	 * @brief Static definition to hold stopping speed for rover.
	 */
	//static const int STOPPING_SPEED = 0;

	/**
	 * @brief Contains the member functions to drive the rover using its motors.
	 */
	//class RoverDriving
	//{
		//private:
			/**
			 * @brief Speed for the rover movement. 360 -> Lowest speed (RoverDriving::LOWEST_SPEED), 480 -> Highest speed (RoverDriving::HIGHEST_SPEED).
			 */
			//int SPEED;

			/**
			 * @brief Flag to hold if RoverDriving is initialized
			 */
			//int ROVERDRIVING_INIT_;

		//public:

			/**
			 * @brief Constructor for RoverDriving class.
			 */
			//explicit RoverLight();

			/**
			 * @brief Destructor for RoverDriving class.
			 */
			//~RoverLight();

			/**
			 * @brief Initializes RoverDriving features.
			 */
			//void initialize();

			/**
			 * @brief Commands the rover to stop.
			 */
			//void stopRover();

			/**
			 * @brief Sets the speed. 360 -> Lowest speed (RoverDriving::LOWEST_SPEED), 480 -> Highest speed (RoverDriving::HIGHEST_SPEED)
			 */
			//void setSpeed (const int speed_setpoint);

			/**
			 * @brief Retrieves the current speed setpoint.
			 */
			//int getSpeed (void);

			/**
			 * @brief Set the speed of the right and left wheels
			 */
			//void differentialDrive(const float angular_speed, const float linear_speed );

			/**
			 * @brief Commands the rover to go forward.
			 */
			//void goForward ();

			/**
			 * @brief Commands the rover to go backward.
			 */
			//void goBackward ();

			/**
			 * @brief Commands the rover to turn right on its spot.
			 */
			//void turnRight ();

			/**
			 * @brief Commands the rover to turn left on its spot.
			 */
			//void turnLeft ();

			/**
			 * @brief Commands the rover to turn forward-left using only one motor.
			 */
			//void turnForwardLeft ();

			/**
			 * @brief Commands the rover to turn forward-right using only one motor.
			 */
			//void turnForwardRight ();

			/**
			 * @brief Commands the rover to turn backward-left using only one motor.
			 */
			//void turnBackwardLeft ();

			/**
			 * @brief Commands the rover to turn backward-right using only one motor.
			 */
			//void turnBackwardRight ();


	//}; 
	
		class RoverLight
	{
		private:
			/**
			 * @brief Flag to indicate light system initialization.
			 */
			int ROVERLIGHT_INIT_;
					

		public:	
			/**
			 * @brief Constructor for RoverLight class.
			 */
			explicit RoverLight();

			/**
			 * @brief Destructor for RoverLight class.
			 */
			~RoverLight();
			/**
			 * @brief Commands the rover to initialize lights .
			 */
			void initialize(void);
			/**
			 * @brief Commands the rover to initialize lights .
			 */
			void intruptsetup(void);
			/**
			 * @brief Commands the rover to turn on driving lights .
			 */
			void on(void);
			/**
			 * @brief Commands the rover to turn off lights.
			 */
			void off(void);
			/**
			 * @brief Commands the rover to turn on backward-lights.
			 */
			void BackW (void);
			/**
			 * @brief Commands the rover to blink the right side indicators.
			 */
			void Blink_R(void);
			/**
			 * @brief Commands the rover to blink the left side indicators.
			 */
			void Blink_L(void);
			/**
			 * @brief Commands the rover to stop blinking.
			 */
			void stop_blink(void);
			};
}



#endif /* API_ROVER_DRIVING_HPP_ */
