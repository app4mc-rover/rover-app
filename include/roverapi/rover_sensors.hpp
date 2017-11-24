/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Sensors API - Interfaces for Rover sensors application development
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++ API 17.11.2017
 *    Gael Blondelle - initial API and parameters
 *
 */

#ifndef API_ROVER_SENSORS_HPP_
#define API_ROVER_SENSORS_HPP_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/* Pins of ultrasonic sensors */
#define SIG0 0   //BCM-17   ->  WiringPi 0   //Same as ECHO0 pin, if some one wants to replace front sr04 with groove sensor
#define SIG1 2   //BCM-27   ->  WiringPi 2   //Same as ECHO1 pin, if some one wants to replace back sr04 with groove sensor

#define TRIG0 7   //BCM-4   ->  WiringPi 7
#define ECHO0 0   //BCM-17   ->  WiringPi 0

#define TRIG1 1   //BCM-18   ->  WiringPi 1
#define ECHO1 2   //BCM-27   ->  WiringPi 2

/* Definitions related to DHT22 sensor */
#define DHT22_RPI_PIN   24  //BCM19  -> wiringPi 24
#define MAX_TIMINGS	    85

/* Definitions related to compass sensor */
#define HMC588L_ADDRESS 0x1E
#define CALIBRATION_DURATION 10000 //compass calibration has a duration of 5 seconds
#define DECLINATION_ANGLE 0.0413 //correction factor for location Paderborn

namespace rover
{
	/**
	 * @brief RoverSensors class contains member functions to setup and read from embedded sensors from rover's RoverSenseLayer sensor layer.
	 * @return void
	 */
	class RoverSensors
	{
		private:

		public:
			/**
			 * @brief Initializes the sensors
			 * @return void
			 */
			void initialize();

			/**
			 * @brief Sets up the HC-SR04 ultrasonic sensor
			 * @return void
			 */
			void setupHCSR04UltrasonicSensor(int sensor_id);

			/**
			 * @brief Reads from HC-SR04 ultrasonic sensor in centimeters. sensor_id 0-> Front ultrasonic sensor, 1-> Rear ultrasonic sensor.
			 * @param sensor_id 0-> Front ultrasonic sensor, 1-> Rear ultrasonic sensor.
			 * @return sensor_val in centimeters
			 */
			int readHCSR04UltrasonicSensor (int sensor_id);

			/**
			 * @brief Sets up the groove ultrasonic sensor
			 * @return void
			 */
			void setupGrooveUltrasonicSensor (void);

			/**
			 * @brief Reads from groove ultrasonic sensor in centimeters. sensor_id 0-> Front ultrasonic sensor, 1-> Rear ultrasonic sensor.
			 * @param sensor_id 0-> Front ultrasonic sensor, 1-> Rear ultrasonic sensor.
			 * @return sensor_val in centimeters
			 */
			int readGrooveUltrasonicSensor (int sensor_id);

			/**
			 * @brief Sets up the infrared sensors and Analog to digital converter
			 * @return void
			 */
			void setupInfraredSensors (void);

			/**
			 * @brief Reads from infrared sensor in centimeters (float).
			 * @param infrared_sensor_id Indicates the channel of which on-board sensor is addressed. 0-> Rear-right, 1-> Rear-left, 2-> Front-right, 3-> Front-left.
			 * @return sensor_val in centimeters (float).
			 */
			float readInfraredSensor (int infrared_sensor_id);

			/**
			 * @brief Sets up the bearing sensor and its I2C interface
			 * @return void
			 */
			void setupBearingSensor (void);

			/**
			 * @brief Reads the bearing value from Bearing sensor (float).
			 * @return bearing_val in degrees (float).
			 */
			float readBearing (void);

			/**
			 * @brief Reads the temperature value in Celcius degrees from DHT22 temperature and humidity sensor (float).
			 * @return temperature_val in Celcius degrees (float).
			 */
			float readTemperature (void);

			/**
			 * @brief Reads the humidity value in percentage from DHT22 temperature and humidity sensor (float).
			 * @return humidity_val in percentage (float).
			 */
			float readHumidity (void);
	};
}



#endif /* API_ROVER_SENSORS_HPP_ */
