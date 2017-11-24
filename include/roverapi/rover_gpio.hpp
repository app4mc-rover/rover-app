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
	class RoverGpio
	{
		private:
			int BUZZER_FREQUENCY;
		public:
			/**
			 * @brief Initializes the RoverGpio functionality
			 * @return void
			 */
			void initialize (void);

			void setBuzzerFrequency (int buzzer_freq);
			int getBuzzerFrequency (void);

			/**
			 * @brief Sets the buzzer to the default frequency.
			 * @return void
			 */
			void setBuzzerOn (void);
			void setBuzzerOff (void);
			void setBuzzerTone (int buzzer_freq);
			void readUserButtonState (void);
			void readShutdownButtonState (void);
	};
}



#endif /* API_ROVER_GPIO_HPP_ */
