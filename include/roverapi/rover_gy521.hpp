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

			/**
			 *  @brief Calibration duration for compass sensor
			 */
			int CALIBRATION_DURATION;

			/**
			 *  @brief Declination angle / correction factor for compass sensor
			 */
			float DECLINATION_ANGLE;

			/**
			 * @brief Calibration variable
			 */
			mutable unsigned int calibration_start;

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
			 * @brief Returns heading in float
			 * @return Bearing
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

			/**
			 * @brief Sets the Calibration Duration
			 * @param period Period to set
			 * @return void
			 */
			void setCalibrationPeriod(const int period);

			/**
			 * @brief Sets the Declination angle
			 * @param angle Declination angle to set
			 * @return void
			 */
			void setDeclinationAngle (const float angle);

		private:

			/**
			 * @brief Inline macro to find minimum of two values regardles of their types (template).
			 * @param a Parameter 1
			 * @param b Parameter 2
			 * @return minimum value
			 */
			template<typename T>
			inline T MINIMUM_ (const T& a, const T& b);

			/**
			 * @brief Inline macro to find maximum of two values regardles of their types (template).
			 * @param a Parameter 1
			 * @param b Parameter 2
			 * @return maximum value
			 */
			template<typename T>
			inline T MAXIMUM_ (const T& a, const T& b);

	};
}




#endif /* ROVERAPI_ROVER_GY521_HPP_ */
