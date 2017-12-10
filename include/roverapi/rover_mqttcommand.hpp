/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTTCommand API - Class for MQTT communication related to rover tasks such as sensor publishing and control data retrieval.
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverMQTTCommand C++API 10.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_MQTTCOMMAND_HPP_
#define ROVERAPI_ROVER_MQTTCOMMAND_HPP_

#include <roverapi/rover_pahomqtt.hpp>

namespace rover
{
	class RoverMQTTCommand: private RoverPahoMQTT
	{
		private:
			char* HOST_NAME;
			int PORT;

		public:
			RoverMQTTCommand (char * host, const int port, const int qos, char * clientID);
			~RoverMQTTCommand ();
	};
}



#endif /* ROVERAPI_ROVER_MQTTCOMMAND_HPP_ */
