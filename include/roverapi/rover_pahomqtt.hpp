/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover PahoMQTT API - Interfaces for Rover PahoMQTT cloud application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created PahoMQTT API 07.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_PAHOMQTT_HPP_
#define ROVERAPI_ROVER_PAHOMQTT_HPP_

#include <roverapi/rover_cloud.hpp>
#include "MQTTClient.h"
#include "MQTTAsync.h"

MQTTClient defaultMQTTClient;

namespace rover
{
	/**
	 * @brief RoverPahoMQTT contains member functions to use rover as a client and to publish / subscribe to Eclipse Paho MQTT server topics.
	 */
	class RoverPahoMQTT : public RoverCloud
	{
		private:
			/**
			 * @brief Host name used for connecting to the Eclipse Paho MQTT server
			 */
			char * HOST_NAME;

			/**
			 * @brief Port used for connecting to the Eclipse Paho MQTT server
			 */
			int PORT;

		public:
			/**
			 * @brief Default constructor for RoverPahoMQTT
			 */
			RoverPahoMQTT();

			/**
			 * @brief Copy constructor for RoverPahoMQTT
			 */
			RoverPahoMQTT(const MQTTClient& my_client);

			/**
			 * @brief Sets private attribute HOST_NAME
			 * @param host_name
			 */
			void setHostName (char * host_name);

			/**
			 * @brief Sets private attribute PORT
			 * @param port
			 */
			void setPort (const int port);



			/**
			 * @brief Connects to the client
			 * @param port
			 */
			int connect (void);

			int publish (void);

			int subscribe (void);

			int unsubscribe (void);

			int destroy (void);
	};
}



#endif /* ROVERAPI_ROVER_PAHOMQTT_HPP_ */
