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
	
	
		class RoverLight
	{
		private:
			/**
			 * @brief Flag to indicate light system initialization.
			 */
			int ROVERLIGHT_INIT_;
			int dimValue;
					

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
			 * @brief Commands the rover to blink the right side indicators.
			 */
			void Blink_R_on(void);
			/**
			 * @brief Commands the rover to blink the left side indicators.
			 */
			void Blink_L_on(void);
			/**
			 * @brief Commands the rover to stop blinking.
			 */
			void stop_blink(void);
			/**
			 * @brief Commands the rover to stop blinking.
			 */
			void dim(void);
			/**
			 * @brief Commands the rover to stop blinking.
			 */
			int dimget(void);
			/**
			 * @brief Commands the rover to stop blinking.
			 */
			void dimset(int);
			};
}



#endif /* API_ROVER_DRIVING_HPP_ */
