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

#include <api/rover_api.hpp>

namespace rover
{
	class RoverDriving : public RoverBase
	{
		public:
			void stop();
			void goForward();
			void goBackward();
			void turnRight();
			void turnLeft();
			void performACC();
			void performParking();
			void performBoothMode1();
			void performBoothMode2();
	};
}


#endif /* API_ROVER_DRIVING_HPP_ */
