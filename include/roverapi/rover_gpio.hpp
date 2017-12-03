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
	 * @brief RoverGpio class provides the member functions related to embedded buzzer, user button, and shutdown button on the rover.
	 */
	class RoverGpio
	{
		private:
			/* Pins */
			/**
			 * @brief Buzzer pin in wiringPi format
			 */
			static const int BUZZER_PIN = 28;			//BCM-20, wiringpi 28

			/**
			 * @brief Shutdown button pin in wiringPi format
			 */
			static const int SHUTDOWN_BUTTON_PIN = 29;	//BCM-21, wiringpi 29

			/**
			 * @brief User button pin in wiringPi format
			 */
			static const int USER_BUTTON_PIN = 27;		//BCM-16, wiringpi 27

			/* Variable members */
			/**
			 * @brief Default buzzer frequency used by setBuzzerOn function.
			 */
			int BUZZER_FREQUENCY;

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
			 * @brief Initializes the RoverGpio functionality: Buzzer and Buttons
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Sets the default buzzer frequency in Hz. Values between 0-1000 Hz are conventionally used for the buzzer frequency.
			 * @param buzzer_freq Buzzer frequency to be set in Hz
			 * @return void
			 */
			void setBuzzerFrequency (int buzzer_freq);

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
			 * @brief Plays the buzzer with custom buzzer frequency.
			 * @param buzzer_freq Buzzer frequency to be set in Hz
			 * @return void
			 */
			void setBuzzerTone (int buzzer_freq);

			/**
			 * @brief Plays the shutdown tone.
			 * @return void
			 */
			void shutdownTone (void);

			/**
			 * @brief Reads the digital value of the user button (USER_BUTTON).
			 * @return user_button_val RoverGpio::LO (low) or RoverGpio::HI (high). Default is LOW and when pressed, user_button_val gives HIGH.
			 */
			int readUserButton (void);

			/**
			 * @brief Reads the digital value of the shutdown button (SHUTDOWN_BUTTON).
			 * @return shutdown_button_val RoverGpio::LO (low) or RoverGpio::HI (high). Default is LOW and when pressed, shutdown_button_val gives HIGH.
			 */
			int readShutdownButton (void);

			/**
			 * @brief Wrapper function to wiringPi's digitalWrite function
			 * @param pin Pin number (int) using wiringPi pin descriptions.
			 * @param value Digital value to be given as output to the pin. LOW -> 0 HIGH -> 1
			 * @return void
			 */
			void wPiDigitalWrite (int pin, int value);

			/**
			 * @brief Wrapper function to wiringPi's digitalRead function.
			 * @param pin Pin number (int) to be read from using wiringPi pin descriptions.
			 * @return value Digital value read from the pin. LOW -> 0 HIGH -> 1
			 */
			int wPiDigitalRead (int pin);

			/**
			 * @brief Wrapper function to wiringPi's pinMode function.
			 * @param pin Pin number (int) to be set
			 * @param set_val Pin Mode to be set for the GPIO pin: RoverGpio::INPUT_, RoverGpio::OUTPUT_, RoverGpio::PWM_OUTPUT_, RoverGpio::GPIO_CLOCK_, RoverGpio::SOFT_PWM_OUTPUT_, RoverGpio::SOFT_TONE_OUTPUT_, RoverGpio::PWM_TONE_OUTPUT_
			 * @return void
			 */
			void wPiPinMode (int pin, int set_val);
	};
}



#endif /* API_ROVER_GPIO_HPP_ */
