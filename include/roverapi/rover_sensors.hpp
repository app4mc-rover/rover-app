/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Sensors API - Interfaces for Rover sensors application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_SENSORS_HPP_
#define API_ROVER_SENSORS_HPP_

#include <roverapi/rover_api.hpp>

namespace rover
{
	class RoverSensors : public RoverBase
	{
		public:
			void initialize();
			void readUltrasonicSensor(int sensor_id);
			void readInfraredSensor(int sensor_id);
			void calibrateBearingSensor();
			void readBearing();
			void readTemperature();
			void readHumidity();
	};
}



#endif /* API_ROVER_SENSORS_HPP_ */
