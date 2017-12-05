/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverQMC5883L API - Interfaces for Rover QMC5883L bearing sensor application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverQMC5883L class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_QMC5883L_HPP_
#define ROVERAPI_ROVER_QMC5883L_HPP_

#include <roverapi/rover_sensor.hpp>

namespace rover
{
	/**
	 * @brief RoverQMC5883L is a class that is inherited from RoverSensor abstract class. RoverQMC5883L class contains member functions and variables to set up and read from QMC5883L bearing sensor that is embedded on the rover.
	 */
	class RoverQMC5883L : public RoverSensor
	{
		private:
			/* Definitions related to compass sensor */
			/**
			 *  @brief Address for compass sensor
			 */
			int QMC5883L_ADDRESS;

			/**
			 *  @brief Calibration duration for compass sensor
			 */
			int CALIBRATION_DURATION;

			/**
			 *  @brief Declination angle / correction factor for compass sensor
			 */
			float DECLINATION_ANGLE;

			/**
			 * @brief Calibration variable for QMC5883L
			 */
			mutable unsigned int calibration_start;

			/**
			 * @brief Flag to hold if RoverQMC5883L is set up
			 */
			int ROVERQMC5883L_SETUP_;

		public:
			/**
			 * @brief Constructor for the RoverQMC5883L class
			 */
			explicit RoverQMC5883L ();

			/**
			 * @brief Destructor for the RoverQMC5883L class
			 */
			~RoverQMC5883L();

			/**
			 * @brief Function to setup QMC5883L sensor
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Reads the bearing value from Bearing sensor QMC5883L (float).
			 * @return bearing_val in degrees (float).
			 */
			float read (void);

			/**
			 * @brief Starts calibration for the bearing sensor
			 * @return void
			 */
			void calibrate (void) const;

			/**
			 * @brief Sets the QMC588L Address
			 * @param address Address to set
			 * @return void
			 */
			void setQMC5883LAddress (const int address);

			/**
			 * @brief Sets the QMC588L Calibration Duration
			 * @param period Period to set
			 * @return void
			 */
			void setQMC5883LCalibrationPeriod(const int period);

			/**
			 * @brief Sets the QMC588L Declination angle
			 * @param angle Declination angle to set
			 * @return void
			 */
			void setQMC5883LDeclinationAngle (const float angle);

			/**
			 * @brief Returns the private attribute QMC588L_ADDRESS
			 * @return address Address to return
			 */
			int getQMC5883LAddress (void);

			/**
			 * @brief Returns the QMC588L Calibration Duration
			 * @return period Period to return
			 */
			int getQMC5883LCalibrationPeriod (void);

			/**
			 * @brief Returns the QMC588L Declination angle
			 * @return angle Declination angle to return
			 */
			float getQMC5883LDeclinationAngle (void);

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



#endif /* ROVERAPI_ROVER_QMC5883L_HPP_ */
