/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverHCSR04 API - Interfaces for Rover ultrasonic sensor application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverHCSR04 class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_HCSR04_HPP_
#define ROVERAPI_ROVER_HCSR04_HPP_

#include <roverapi/rover_sensor.hpp>

/**
 *  @brief HC-SR04 Sensor Trigger Pin for Front Sensor Socket (in wiringPi format)
 */
static const int TRIG0 = 7; //BCM-4   ->  WiringPi 7
/**
 *  @brief HC-SR04 Sensor Echo Pin for Front Sensor Socket (in wiringPi format)
 */
static const int ECHO0 = 0; //BCM-17   ->  WiringPi 0

/**
 *  @brief HC-SR04 Sensor Trigger Pin for Rear Sensor Socket (in wiringPi format)
 */
static const int TRIG1 = 1; //BCM-18   ->  WiringPi 1
/**
 *  @brief HC-SR04 Sensor Echo Pin for Rear Sensor Socket (in wiringPi format)
 */
static const int ECHO1 = 2; //BCM-27   ->  WiringPi 2

namespace rover
{
	/**
	 * @brief RoverHCSR04 is a class that is inherited from RoverSensor abstract class. RoverHCSR04 class contains member functions and variables to set up and read from HCSR-04 ultrasonic sensors that are embedded on the rover.
	 */
	class RoverHCSR04 : public RoverSensor
	{
		private:
			/**
			 * @brief Sensor Identification
			 */
			RoverSensorID_t sensorID;

			/**
			 * @brief Trigger pin
			 */
			int trigPin;

			/**
			 * @brief Echo pin
			 */
			int echoPin;

			/**
			 * @brief Flag to hold if RoverHCSR04 is set up
			 */
			int ROVERHCSR04_SETUP_;

		public:
			/**
			 * @brief Constructor for the RoverHCSR04 class
			 * @param sensor_id Sensor to address
			 */
			explicit RoverHCSR04 (const RoverSensorID_t sensor_id);

			/**
			 * @brief Destructor for the RoverHCSR04 class
			 */
			~RoverHCSR04();

			/**
			 * @brief Function to setup HCSR04 sensor
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Member function to read from HC-SR04 ultrasonic sensor given its sensor id.
			 * @return sensor_val Sensor value read in centimeters from sensor
			 */
			float read (void);

			/**
			 * @brief Sets the private attribute trigPin for HCSR04
			 * @param trig_pin Pin in wiringPi format for HCSR04
			 * @return void
			 */
			void setTrigPin (const int trig_pin);

			/**
			 * @brief Returns the private attribute trigPin for HCSR04
			 * @return trig_pin Pin in wiringPi format for HCSR04
			 */
			int getTrigPin (void);

			/**
			 * @brief Sets the private attribute echoPin for HCSR04
			 * @param echo_pin Pin in wiringPi format for HCSR04
			 * @return void
			 */
			void setEchoPin (const int echo_pin);

			/**
			 * @brief Returns the private attribute echoPin for HCSR04
			 * @return echo_pin Pin in wiringPi format for HCSR04
			 */
			int getEchoPin (void);

	};
}



#endif /* ROVERAPI_ROVER_HCSR04_HPP_ */
