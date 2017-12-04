/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverHMC5883L API - Interfaces for Rover HMC5883L bearing sensor application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverHMC5883L class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_HMC5883L_HPP_
#define ROVERAPI_ROVER_HMC5883L_HPP_

#include <roverapi/rover_sensor.hpp>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

namespace rover
{
	/**
	 * @brief RoverHMC5883L is a class that is inherited from RoverSensor abstract class. RoverHMC5883L class contains member functions and variables to set up and read from HMC5883L bearing sensor that is embedded on the rover.
	 */
	class RoverHMC5883L : public RoverSensor
	{
		private:
			/* Definitions related to compass sensor */
			/**
			 *  @brief Address for compass sensor
			 */
			int HMC588L_ADDRESS;

			/**
			 *  @brief Calibration duration for compass sensor
			 */
			int CALIBRATION_DURATION;

			/**
			 *  @brief Declination angle / correction factor for compass sensor
			 */
			float DECLINATION_ANGLE;

			/**
			 * @brief Calibration variable for HMC5883L
			 */
			mutable unsigned int calibration_start;

			/**
			 * @brief Flag to hold if HMC5883L is set up
			 */
			int ROVERHMC5883L_SETUP_;

		public:
			/**
			 * @brief Constructor for the RoverHMC5883L class
			 */
			explicit RoverHMC5883L ();

			/**
			 * @brief Destructor for the RoverHMC5883L class
			 */
			~RoverHMC5883L();

			/**
			 * @brief Function to setup HMC5883L sensor
			 * @return void
			 */
			void setup (void);

			/**
			 * @brief Reads the bearing value from Bearing sensor Sunfounder HMC5883L (float).
			 * @return bearing_val in degrees (float).
			 */
			float read (void);

			/**
			 * @brief Starts calibration for the bearing sensor
			 * @return void
			 */
			void calibrate (void) const;

			/**
			 * @brief Sets the HMC588L Address
			 * @param address Address to set
			 * @return void
			 */
			void setHMC588LAddress (const int address);

			/**
			 * @brief Sets the HMC588L Calibration Duration
			 * @param period Period to set
			 * @return void
			 */
			void setHMC588LCalibrationPeriod(const int period);

			/**
			 * @brief Sets the HMC588L Declination angle
			 * @param angle Declination angle to set
			 * @return void
			 */
			void setHMC588LDeclinationAngle (const float angle);

			/**
			 * @brief Returns the private attribute HMC588L_ADDRESS
			 * @return address Address to return
			 */
			int getHMC588LAddress (void);

			/**
			 * @brief Returns the HMC588L Calibration Duration
			 * @return period Period to return
			 */
			int getHMC588LCalibrationPeriod (void);

			/**
			 * @brief Returns the HMC588L Declination angle
			 * @return angle Declination angle to return
			 */
			float getHMC588LDeclinationAngle (void);

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



#endif /* ROVERAPI_ROVER_HMC5883L_HPP_ */
