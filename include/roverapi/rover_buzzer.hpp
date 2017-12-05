/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    RoverBuzzer API - Interfaces for Rover buzzer application development
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++API 05.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_BUZZER_HPP_
#define ROVERAPI_ROVER_BUZZER_HPP_

#include <roverapi/rover_gpio.hpp>

namespace rover
{
	/**
	 * @brief RoverBuzzer class provides the member functions related to embedded buzzer on the rover.
	 */
	class RoverBuzzer : public RoverGpio
	{
		private:
			/* Pins */
			/**
			 * @brief Embedded buzzer pin on rover in wiringPi format
			 */
			static const int DEFAULT_BUZZER_PIN = 28; //BCM-20, wiringpi 28

			/**
			 * @brief Buzzer pin in wiringPi format
			 */
			int BUZZER_PIN;

			/* Variable members */
			/**
			 * @brief Default buzzer frequency used by setBuzzerOn function.
			 */
			int BUZZER_FREQUENCY;

			/**
			 * @brief Flag to hold if RoverBuzzer is initialized
			 */
			int ROVERBUZZER_INIT_;

		public:

			/**
			 * @brief RoverBuzzer classs constructor
			 */
			RoverBuzzer();

			/**
			 * @brief Initializes the RoverBuzzer that is instantiated.
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Sets the default buzzer frequency in Hz. Values between 0-1000 Hz are conventionally used for the buzzer frequency.
			 * @param buzzer_freq Buzzer frequency to be set in Hz
			 * @return void
			 */
			void setBuzzerFrequency (const int buzzer_freq);

			/**
			 * @brief Retrieves the default buzzer frequency in Hz.
			 * @return buzzer_freq Default buzzer frequency in Hz.
			 */
			int getBuzzerFrequency (void);

			/**
			 * @brief Plays the buzzer with the default frequency.
			 * @return void
			 */
			void setBuzzerOn (void);

			/**
			 * @brief Turns off the buzzer.
			 * @return void
			 */
			void setBuzzerOff (void);

			/**
			 * @brief Overrides the default buzzer pin in wiringPi format
			 * @param pin in wiringPi format
			 * @return void
			 */
			void setBuzzerPin (const int pin);

			/**
			 * @brief Plays the buzzer with custom buzzer frequency.
			 * @param buzzer_freq Buzzer frequency to be set in Hz
			 * @return void
			 */
			void setBuzzerTone (const int buzzer_freq);

			/**
			 * @brief Plays the shutdown tone.
			 * @return void
			 */
			void shutdownTone (void);
	};
}


#endif /* ROVERAPI_ROVER_BUZZER_HPP_ */
