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
 * Disclaimer:
 *    Adapted from https://github.com/OmarAflak/GY-521-Raspberry-Pi-C-/
 *
 */

#ifndef ROVERAPI_ROVER_GY521_HPP_
#define ROVERAPI_ROVER_GY521_HPP_

#include <roverapi/rover_sensor.hpp>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


namespace rover
{
	/**
	 * @brief RoverGY521 is a class that is inherited from RoverSensor abstract class. RoverGY521 class contains member functions and variables to set up and read from GY521 accelerometer that is embedded on the rover.
	 */
	class RoverGY521 : public RoverSensor
	{
		private:
			/**
			 * @brief Flag to hold if RoverGY521 is initialized or not
			 */
			int ROVERGY521_SETUP_;

			/**
			 * @brief I2C address for overriding
			 */
			int i2CAddress;

		public:
			/**
			 * @brief Default Constructor for the RoverGY521 class
			 */
			explicit RoverGY521 ();

			/**
			 * @brief Assigning Constructor for the RoverGY521 class
			 * @param custom_i2c_address Custom I2C address to initialize RoverGY521 sensor with.
			 */
			explicit RoverGY521 (const int custom_i2c_address);

			/**
			 * @brief Destructor for the RoverGY521 class
			 */
			~RoverGY521();

			/**
			 * @brief Function to setup RoverGY521 sensor
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Overrided unused read function from RoverSensor class.
			 */
			float read (void);

			/**
			 * @brief Returns gyroscope output for X-axis
			 * @return gyproscope_x_output
			 */
			int8_t getGyroX (void);

			/**
			 * @brief Returns gyroscope output for Y-axis
			 * @return gyproscope_y_output
			 */
			int8_t getGyroY (void);

			/**
			 * @brief Returns gyroscope output for Z-axis
			 * @return gyproscope_z_output
			 */
			int8_t getGyroZ (void);

			/**
			 * @brief Returns accelerometer output for X-axis
			 * @return accelerometer_x_output
			 */
			int8_t getAccelX (void);

			/**
			 * @brief Returns accelerometer output for Y-axis
			 * @return accelerometer_y_output
			 */
			int8_t getAccelY (void);

			/**
			 * @brief Returns accelerometer output for Z-axis
			 * @return accelerometer_z_output
			 */
			int8_t getAccelZ (void);

			/**
			 * @brief Returns angle in the X-axis
			 * @return angle_x
			 */
			float getAngleX (void);

			/**
			 * @brief Returns angle in the Y-axis
			 * @return angle_y
			 */
			float getAngleY (void);

			/**
			 * @brief Returns angle in the Z-axis
			 * @return angle_z
			 */
			float getAngleZ (void);

	};
}




#endif /* ROVERAPI_ROVER_GY521_HPP_ */
