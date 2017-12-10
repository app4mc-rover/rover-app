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
 *                                          improved PahoMQTT API 10.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_PAHOMQTT_HPP_
#define ROVERAPI_ROVER_PAHOMQTT_HPP_

#include <roverapi/rover_cloud.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "MQTTClient.h"
#include "MQTTAsync.h"

using namespace std;

namespace rover
{

	typedef struct
	{
		char * clientID;
		char * topic;
		char * payload;
		int			qos;		// 1
		long int	timeout;	// Such as 10000L usec
	} RoverMQTT_Configure_t;

	typedef struct
	{
		int data_ready;
		char * data;
	} RoverMQTT_SubscribeData_t;

	typedef struct
	{
		int f_mqtt_disconnect_finished;
		int f_mqtt_finished;
		int f_mqtt_subscribed;
		int f_mqtt_publish_successful;
	} RoverMQTT_StatusFlags_t;

	typedef struct
	{
		int rc_publish;
		int rc_subscribe;
		int rc_unsubscribe;
	} RoverMQTT_ReturnCodes_t;

	/**
	 * @brief RoverPahoMQTT contains member functions to use rover as a client and to publish / subscribe to Eclipse Paho MQTT server topics.
	 * This class is not thread-safe.
	 */
	class RoverPahoMQTT : public RoverCloud
	{
		public:

			char * read (void);

			int isDataReady (void);

		private:


			/**
			 * @brief Host name used for connecting to the Eclipse Paho MQTT server
			 */
			char * HOST_NAME;

			/**
			 * @brief Port used for connecting to the Eclipse Paho MQTT server
			 */
			int PORT;

			RoverMQTT_SubscribeData_t defaultRoverSubscribeData;

			RoverMQTT_Configure_t defaultRoverMQTTConfigure;

			RoverMQTT_StatusFlags_t defaultRoverMQTTFlags;

			RoverMQTT_ReturnCodes_t defaultReturnCodes;


			/* Internal attributes */
			MQTTAsync client;

			/**
			 * @brief Connect options
			 */
			MQTTAsync_connectOptions conn_opts;

			/**
			 * @brief Disconnect options
			 */
			MQTTAsync_disconnectOptions disc_opts;


			volatile MQTTAsync_token deliveredtoken;



		public:

			~RoverPahoMQTT ();

			/**
			 * @brief Copy constructor for RoverPahoMQTT
			 */
			RoverPahoMQTT (char * host_name, int port, RoverMQTT_Configure_t MQTT_Configure);

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

			void setPayload (char * payload);

			void setTopic (char * topic);



			int publish (void);

			int subscribe (void);

			int unsubscribe (void);


		private:
			void flushFlags (void);

			void constructAddress (char* string);

			/* Internal callbacks */
			/* Related to publisher */
			static void onPublisherConnect_Redirect (void* context, MQTTAsync_successData* response);
			void onPublisherConnect (MQTTAsync_successData* response);

			static void onPublisherSend_Redirect (void* context, MQTTAsync_successData* response);
			void onPublisherSend (MQTTAsync_successData* response);

			/* Related to subscriber */
			static void onSubscriberConnect_Redirect (void* context, MQTTAsync_successData* response);
			void onSubscriberConnect (MQTTAsync_successData* response);

			static int onSubscriberMessageArrived_Redirect (void *context, char *topicName, int topicLen, MQTTAsync_message *message);
			int onSubscriberMessageArrived (char *topicName, int topicLen, MQTTAsync_message *message);

			static void onSubscribe_Redirect (void* context, MQTTAsync_successData* response);
			void onSubscribe (MQTTAsync_successData* response);

			static void onSubscribeFailure_Redirect (void* context, MQTTAsync_failureData* response);
			void onSubscribeFailure (MQTTAsync_failureData* response);

			/* Related to both */
			static void onConnectFailure_Redirect (void* context, MQTTAsync_failureData* response);
			void onConnectFailure (MQTTAsync_failureData* response);

			static void onConnectionLost_Redirect (void* context, char *cause);
			void onConnectionLost (char *cause);

			static void onDisconnect_Redirect (void* context, MQTTAsync_successData* response);
			void onDisconnect (MQTTAsync_successData* response);

	};
}



#endif /* ROVERAPI_ROVER_PAHOMQTT_HPP_ */
