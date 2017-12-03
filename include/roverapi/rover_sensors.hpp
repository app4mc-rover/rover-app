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
 *    										initial QMC5883L driver implemented 30.11.2017
 *    David Schmelter, Fraunhofer IEM - compass sensor initial implementation
 *    Gael Blondelle, Eclipse Foundation - initial API and parameters
 *
 */

#ifndef API_ROVER_SENSORS_HPP_
#define API_ROVER_SENSORS_HPP_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


//To debug the application, uncomment the following
//#define DEBUG 1


namespace rover
{
	/**
	 * @brief RoverSensors class contains member functions to setup and read from embedded sensors from rover's RoverSenseLayer sensor layer.
	 */
	class RoverSensors
	{
		private:
			/* Pins of ultrasonic sensors */
			/**
			 * @brief Groove Sensor Pin for Front Sensor Socket (in wiringPi format)
			 */
			static const int SIG0 = 0; //BCM-17   ->  WiringPi 0   //Same as ECHO0 pin, if some one wants to replace front sr04 with groove sensor

			/**
			 *  @brief Groove Sensor Pin for Rear Sensor Socket (in wiringPi format)
			 */
			static const int SIG1 = 2; //BCM-27   ->  WiringPi 2   //Same as ECHO1 pin, if some one wants to replace back sr04 with groove sensor

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

			/* Definitions related to DHT22 sensor */
			/**
			 *  @brief DHT22 sensor pin in wiringPi format
			 */
			static const int DHT22_RPI_PIN = 24;  //BCM19  -> wiringPi 24
			static const int MAX_TIMINGS = 85;

			/* Definitions related to compass sensor */
			/**
			 *  @brief Address for compass sensor
			 */
			int HMC588L_ADDRESS = 0x1E; //0x1E;
			/**
			 *  @brief Calibration duration for compass sensor
			 */
			int CALIBRATION_DURATION = 10000; //compass calibration has a duration of 5 seconds
			/**
			 *  @brief Declination angle / correction factor for compass sensor
			 */
			float DECLINATION_ANGLE = 0.0413; //correction factor for location Paderborn

			/**
			 * @brief Calibration variable for HMC588L
			 */
			mutable unsigned int calibration_start;

		public:

			/* Rover Sensor IDs */
			/**
			 * @brief Static definition to indicate rover's front ultrasonic sensor socket
			 */
			static const int ROVER_FRONT = 0;

			/**
			 * @brief Static definition to indicate rover's rear ultrasonic sensor socket
			 */
			static const int ROVER_REAR = 1;

			/* Rover Infrared Sensor IDs */
			/**
			 * @brief Static definition to indicate rover's rear-right infrared sensor
			 */
			static const int ROVER_REAR_RIGHT = 0;

			/**
			 * @brief Static definition to indicate rover's rear-left infrared sensor
			 */
			static const int ROVER_REAR_LEFT = 1;

			/**
			 * @brief Static definition to indicate rover's front-right infrared sensor
			 */
			static const int ROVER_FRONT_RIGHT = 2;

			/**
			 * @brief Static definition to indicate rover's front-left infrared sensor
			 */
			static const int ROVER_FRONT_LEFT = 3;

			/**
			 * @brief Initializes the sensors
			 * @return void
			 */
			void initialize();

			/***
			 * @brief Constructor for the RoverSensors class.
			 */
			explicit RoverSensors();

			/**
			 * @brief Sets up the HC-SR04 ultrasonic sensor
			 * @return void
			 */
			void setupHCSR04UltrasonicSensor(const int sensor_id);

			/**
			 * @brief Reads from HC-SR04 ultrasonic sensor in centimeters. sensor_id RoverSensors::ROVER_FRONT: Front ultrasonic sensor, RoverSensors::ROVER_REAR: Rear ultrasonic sensor.
			 * @param sensor_id RoverSensors::ROVER_FRONT: Front ultrasonic sensor, RoverSensors::ROVER_REAR: Rear ultrasonic sensor.
			 * @return sensor_val in centimeters
			 */
			int readHCSR04UltrasonicSensor (const int sensor_id);

			/**
			 * @brief Sets up the groove ultrasonic sensor
			 * @return void
			 */
			void setupGrooveUltrasonicSensor (void);

			/**
			 * @brief Reads from groove ultrasonic sensor in centimeters. sensor_id RoverSensors::ROVER_FRONT: Front ultrasonic sensor, RoverSensors::ROVER_REAR: Rear ultrasonic sensor.
			 * @param sensor_id RoverSensors::ROVER_FRONT: Front ultrasonic sensor, RoverSensors::ROVER_REAR: Rear ultrasonic sensor.
			 * @return sensor_val in centimeters
			 */
			int readGrooveUltrasonicSensor (const int sensor_id);

			/**
			 * @brief Sets up the infrared sensors and Analog to digital converter
			 * @return void
			 */
			void setupInfraredSensors (void);

			/**
			 * @brief Reads from infrared sensor in centimeters (float).
			 * @param infrared_sensor_id Indicates the channel of which on-board sensor is addressed. RoverSensors::ROVER_REAR_RIGHT: Rear-right, RoverSensors::ROVER_REAR_LEFT: Rear-left, RoverSensors::ROVER_FRONT_RIGHT: Front-right, RoverSensors::ROVER_FRONT_LEFT: Front-left.
			 * @return sensor_val in centimeters (float).
			 */
			float readInfraredSensor (const int infrared_sensor_id);

			/**
			 * @brief Sets up the bearing sensor Sunfounder HMC5883L and its I2C interface
			 * @return void
			 */
			void setupBearingHMC5883L (void);

			/**
			 * @brief Starts calibration for the bearing sensor
			 * @return void
			 */
			void calibrateBearingSensor (void) const;

			/**
			 * @brief Sets the HMC588L Address
			 * @return void
			 */
			void setHMC588LAddress (const int address);

			/**
			 * @brief Sets the HMC588L Caliration Duration
			 * @return void
			 */
			void setHMC588LCalibrationPeriod(const int period);

			/**
			 * @brief Sets the HMC588L Declination angle
			 * @return void
			 */
			void setHMC588LDeclinationAngle (const float angle);

			/**
			 * @brief Reads the bearing value from Bearing sensor Sunfounder HMC5883L (float).
			 * @return bearing_val in degrees (float).
			 */
			float readBearingHMC5883L (void);

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

			/**
			 * @brief Sets up the bearing sensor with QMC5883L chip and its I2C interface
			 * @return void
			 */
			void setupBearingQMC5883L (void);

			/**
			 * @brief Reads the bearing value from Bearing sensor with QMC5883L chip (float).
			 * @return bearing_val in degrees (float).
			 */
			float readBearingQMC5883L (void);
	};
}



#endif /* API_ROVER_SENSORS_HPP_ */
