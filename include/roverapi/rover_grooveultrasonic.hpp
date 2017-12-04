/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverGrooveUltrasonic API - Interfaces for Rover Groove ultrasonic sensor application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverGrooveUltrasonic class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_GROOVEULTRASONIC_HPP_
#define ROVERAPI_ROVER_GROOVEULTRASONIC_HPP_

#include <roverapi/rover_sensor.hpp>

/**
 * @brief Groove Sensor Pin for Front Sensor Socket (in wiringPi format)
 */
static const int SIG0 = 0; //BCM-17   ->  WiringPi 0   //Same as ECHO0 pin, if some one wants to replace front sr04 with groove sensor

/**
 *  @brief Groove Sensor Pin for Rear Sensor Socket (in wiringPi format)
 */
static const int SIG1 = 2; //BCM-27   ->  WiringPi 2   //Same as ECHO1 pin, if some one wants to replace back sr04 with groove sensor

namespace rover
{
	/**
	 * @brief RoverGrooveUltrasonic is a class that is inherited from RoverSensor abstract class. RoverGrooveUltrasonic class contains member functions and variables to set up and read from Groove ultrasonic sensors that are embedded on the rover.
	 */
	class RoverGrooveUltrasonic : public RoverSensor
	{
		private:
			/**
			 * @brief Sensor Identification
			 */
			RoverSensorID_t sensorID;

			/**
			 * @brief Signal pin
			 */
			int sigPin;

		public:
			/**
			 * @brief Constructor for the RoverGrooveUltrasonic class
			 * @param sensor_id Sensor to address
			 */
			explicit RoverGrooveUltrasonic (const RoverSensorID_t sensor_id);

			/**
			 * @brief Destructor for the RoverGrooveUltrasonic class
			 */
			~RoverGrooveUltrasonic();

			/**
			 * @brief Function to setup Groove Ultrasonic sensor
			 * @return void
			 */
			void setup (void);

			/**
			 * @brief Member function to read from Groove ultrasonic sensor given its sensor id.
			 * @return sensor_val Sensor value read in centimeters from sensor
			 */
			float read (void);

			/**
			 * @brief Sets the private attribute sigPin for Groove Ultrasonic Sensor
			 * @param sig_pin Pin in wiringPi format for Groove Ultrasonic Sensor
			 * @return void
			 */
			void setSigPin (const int sig_pin);

			/**
			 * @brief Returns the private attribute sigPin for Groove Ultrasonic Sensor
			 * @return sig_pin Pin in wiringPi format for Groove Ultrasonic Sensor
			 */
			int getSigPin (void);
	};
}



#endif /* ROVERAPI_ROVER_GROOVEULTRASONIC_HPP_ */
