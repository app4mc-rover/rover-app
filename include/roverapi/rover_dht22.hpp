/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverDHT22 API - Interfaces for Rover DHT22 temperature and humidity sensor application development
 *     Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverDHT22 class 04.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_DHT22_HPP_
#define ROVERAPI_ROVER_DHT22_HPP_

#include <roverapi/rover_sensor.hpp>

/* Definitions related to DHT22 sensor */
/**
 *  @brief DHT22 sensor pin in wiringPi format
 */
static const int DHT22_RPI_PIN = 24;  //BCM19  -> wiringPi 24

static const int MAX_TIMINGS = 85;

namespace rover
{
	/**
	 * @brief RoverDHT22 is a class that is inherited from RoverSensor abstract class. RoverDHT22 class contains member functions and variables to set up and read from DHT22 temperature and humidity sensor that is embedded on the rover.
	 */
	class RoverDHT22 : public RoverSensor
	{
		private:
			/**
			 * @brief Pin in wiringPi format for DHT22 signal
			 */
			int DHT22Pin;

			/**
			 * @brief Flag to hold if DHT22 is set up
			 */
			int DHT22_SETUP_;

		public:
			/**
			 * @brief Constructor for the RoverDHT22 class
			 */
			explicit RoverDHT22 ();

			/**
			 * @brief Destructor for the RoverDHT22 class
			 */
			~RoverDHT22();

			/**
			 * @brief Function to setup DHT22 sensor
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Reads the temperature value from DHT22 (float).
			 * @return temperature_val Temperature in Celcius (float).
			 */
			float read (void);

			/**
			 * @brief Reads the temperature value from DHT22 (float).
			 * @return temperature_val Temperature in Celcius (float).
			 */
			float readTemperature (void);

			/**
			 * @brief Reads the humidity value from DHT22 (float).
			 * @return humidity Humidity in Percentage (float).
			 */
			float readHumidity (void);

			/**
			 * @brief Sets the DHT22 pin in wiringPi format
			 * @param pin Pin to set as DHT22's signal in wiringPi format
			 * @return void
			 */
			void setDHT22Pin (const int pin);

			/**
			 * @brief Returns the DHT22 pin in wiringPi format
			 * @return Pin to return as DHT22's signal in wiringPi format
			 */
			int getDHT22Pin (void);

	};
}


#endif /* ROVERAPI_ROVER_DHT22_HPP_ */
