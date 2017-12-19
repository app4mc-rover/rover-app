/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Driving API - Interfaces for Rover driving application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_DRIVING_HPP_
#define API_ROVER_DRIVING_HPP_

namespace rover
{
	/* Rover driving speeds */
	/**
	 * @brief Static definition to hold lowest driving speed for rover.
	 */
	static const int LOWEST_SPEED = 360;

	/**
	 * @brief Static definition to hold highest driving speed for rover.
	 */
	static const int HIGHEST_SPEED = 480;

	/**
	 * @brief Static definition to hold stopping speed for rover.
	 */
	static const int STOPPING_SPEED = 0;

	/**
	 * @brief Contains the member functions to drive the rover using its motors.
	 */
	class RoverDriving
	{
		private:
			/**
			 * @brief Speed for the rover movement. 360 -> Lowest speed (RoverDriving::LOWEST_SPEED), 480 -> Highest speed (RoverDriving::HIGHEST_SPEED).
			 */
			int SPEED;

			/**
			 * @brief Flag to hold if RoverDriving is initialized
			 */
			int ROVERDRIVING_INIT_;

		public:

			/**
			 * @brief Constructor for RoverDriving class.
			 */
			explicit RoverDriving();

			/**
			 * @brief Destructor for RoverDriving class.
			 */
			~RoverDriving();

			/**
			 * @brief Initializes RoverDriving features.
			 */
			void initialize();

			/**
			 * @brief Commands the rover to stop.
			 */
			void stopRover();

			/**
			 * @brief Sets the speed. 360 -> Lowest speed (RoverDriving::LOWEST_SPEED), 480 -> Highest speed (RoverDriving::HIGHEST_SPEED)
			 */
			void setSpeed (const int speed_setpoint);

			/**
			 * @brief Retrieves the current speed setpoint.
			 */
			int getSpeed (void);

			/**
			 * @brief Commands the rover to go forward.
			 */
			void goForward ();

			/**
			 * @brief Commands the rover to go backward.
			 */
			void goBackward ();

			/**
			 * @brief Commands the rover to turn right on its spot.
			 */
			void turnRight ();

			/**
			 * @brief Commands the rover to turn left on its spot.
			 */
			void turnLeft ();

			/**
			 * @brief Commands the rover to turn forward-left using only one motor.
			 */
			void turnForwardLeft ();

			/**
			 * @brief Commands the rover to turn forward-right using only one motor.
			 */
			void turnForwardRight ();

			/**
			 * @brief Commands the rover to turn backward-left using only one motor.
			 */
			void turnBackwardLeft ();

			/**
			 * @brief Commands the rover to turn backward-right using only one motor.
			 */
			void turnBackwardRight ();

	};
}


#endif /* API_ROVER_DRIVING_HPP_ */
