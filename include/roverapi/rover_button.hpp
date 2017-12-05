/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    RoverButton API - Interfaces for Rover button application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++API 05.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_BUTTON_HPP_
#define ROVERAPI_ROVER_BUTTON_HPP_


#include <roverapi/rover_gpio.hpp>

namespace rover
{

	/**
	 * @brief Button type
	 */
	enum RoverButtonType_t { USER_BUTTON = 0,
							 SHUTDOWN_BUTTON = 1,
							 CUSTOM_BUTTON = 2};

	/**
	 * @brief RoverButton class contains member functions to access Shutdown button, User button, and customly installed buttons on the rover.
	 */
	class RoverButton : public RoverGpio
	{
		private:
			/**
			 * @brief Shutdown button pin in wiringPi format
			 */
			static const int SHUTDOWN_BUTTON_PIN = 29;	//BCM-21, wiringpi 29

			/**
			 * @brief User button pin in wiringPi format
			 */
			static const int USER_BUTTON_PIN = 27;		//BCM-16, wiringpi 27

			/**
			 * @brief Button pin
			 */
			int buttonPin;

			/**
			 * @brief Button type
			 */
			RoverButtonType_t buttonType;

			/**
			 * @brief Flag to hold if RoverButton is set up
			 */
			int ROVERBUTTON_INIT_;

		public:
			/**
			 * @brief (assigning) Constructor for RoverButton
			 */
			RoverButton (const RoverButtonType_t button_type);

			/**
			 * @brief RoverButton initializer
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Reads the digital value of the a button
			 * @return val RoverGpio::LO (low) or RoverGpio::HI (high). Embedded shutdown and user buttons are pulled-up. Therefore, when the button is pressed a LOW (0) is returned.
			 */
			int readButton (void);

			/**
			 * @brief Overrides the default button pin used in wiringPi format
			 * @param button_pin
			 * @return void
			 */
			void setButtonPin (const int button_pin);
	};
}


#endif /* ROVERAPI_ROVER_BUTTON_HPP_ */
