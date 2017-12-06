/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Gpio API - Interfaces for Rover general purpose I/O application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_GPIO_HPP_
#define API_ROVER_GPIO_HPP_

namespace rover
{
	/**
	 * @brief RoverGpio class provides the member functions related to basic GPIO operations. This class wraps wiringPi library.
	 */
	class RoverGpio
	{
		public:

			/* Pin Modes */
			/**
			 * @brief Static definition for INPUT pin mode for wPiPinMode function
			 */
			static const int INPUT_ = 0;

			/**
			 * @brief Static definition for OUTPUT pin mode for wPiPinMode function
			 */
			static const int OUTPUT_ = 1;

			/**
			 * @brief Static definition for PWM_OUTPUT pin mode for wPiPinMode function
			 */
			static const int PWM_OUTPUT_ = 2;

			/**
			 * @brief Static definition for GPIO_CLOCK pin mode for wPiPinMode function
			 */
			static const int GPIO_CLOCK_ = 3;

			/**
			 * @brief Static definition for SOFT_PWM_OUTPUT pin mode for wPiPinMode function
			 */
			static const int SOFT_PWM_OUTPUT_ = 4;

			/**
			 * @brief Static definition for SOFT_TONE_OUTPUT pin mode for wPiPinMode function
			 */
			static const int SOFT_TONE_OUTPUT_ = 5;

			/**
			 * @brief Static definition for PWM_TONE_OUTPUT pin mode for wPiPinMode function
			 */
			static const int PWM_TONE_OUTPUT_ = 6;

			/* Voltage levels */
			/**
			 * @brief Static definition for digital voltage level : LOW
			 */
			static const int LO = 0;

			/**
			 * @brief Static definition for digital voltage level : HIGH
			 */
			static const int HI = 1;

			/**
			 * @brief Constructor function for the RoverGpio class
			 * @return void
			 */
			explicit RoverGpio();


			/**
			 * @brief Wrapper function to wiringPi's digitalWrite function
			 * @param pin Pin number (int) using wiringPi pin descriptions.
			 * @param value Digital value to be given as output to the pin. LOW -> 0 HIGH -> 1
			 * @return void
			 */
			void wPiDigitalWrite (const int pin, const int value);

			/**
			 * @brief Wrapper function to wiringPi's digitalRead function.
			 * @param pin Pin number (int) to be read from using wiringPi pin descriptions.
			 * @return value Digital value read from the pin. LOW -> 0 HIGH -> 1
			 */
			int wPiDigitalRead (const int pin);

			/**
			 * @brief Wrapper function to wiringPi's pinMode function.
			 * @param pin Pin number (int) to be set
			 * @param set_val Pin Mode to be set for the GPIO pin: RoverGpio::INPUT_, RoverGpio::OUTPUT_, RoverGpio::PWM_OUTPUT_, RoverGpio::GPIO_CLOCK_, RoverGpio::SOFT_PWM_OUTPUT_, RoverGpio::SOFT_TONE_OUTPUT_, RoverGpio::PWM_TONE_OUTPUT_
			 * @return void
			 */
			void wPiPinMode (const int pin, const int set_val);

			/**
			 * @brief Wrapper function to wiringPi's softToneCreate function. Sets up a pin to have PWM  soft tone output.
			 * @param pin Pin number (int) to be set
			 * @return void
			 */
			void wPiSoftToneCreate (const int pin);

			/**
			 * @brief Wrapper function to wiringPi's softToneWrite function. Sets up a pin to have PWM  soft tone output.
			 * @param pin Pin number (int) to be set
			 * @param tone Tone frequency to create in Hz
			 * @return void
			 */
			void wPiSoftToneWrite (const int pin, const int tone);

			/**
			 * @brief Delay function. Blocks some time in milliseconds
			 * @param period_ms Period in milliseconds
			 * @return void
			 */
			void wPiDelay (const unsigned int period_ms);
	};
}



#endif /* API_ROVER_GPIO_HPP_ */
