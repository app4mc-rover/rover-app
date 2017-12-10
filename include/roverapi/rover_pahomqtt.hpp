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

	/**
	 * @brief Data type to store initial configuration information for MQTT communication.
	 */
	typedef struct
	{
		char * clientID;
		char * topic;
		char * payload;
		int			qos;		// 1
		long int	timeout;	// Such as 10000L usec
	} RoverMQTT_Configure_t;

	/**
	 * @brief Data type to store subscribed data
	 */
	typedef struct
	{
		int data_ready;
		char * data;
	} RoverMQTT_SubscribeData_t;

	/**
	 * @brief Data type to store internal flags for MQTT communication
	 */
	typedef struct
	{
		int f_mqtt_disconnect_finished;
		int f_mqtt_finished;
		int f_mqtt_subscribed;
		int f_mqtt_publish_successful;
	} RoverMQTT_StatusFlags_t;

	/**
	 * @brief Data type to store return codes that belong to member functions in RoverPahoMQTT class.
	 */
	typedef struct
	{
		int rc_publish;
		int rc_subscribe;
		int rc_unsubscribe;
	} RoverMQTT_ReturnCodes_t;

	/**
	 * @brief RoverPahoMQTT contains member functions to use rover as a client and to publish / subscribe to Eclipse Paho MQTT server topics.
	 */
	class RoverPahoMQTT : public RoverCloud
	{
		protected:
			/**
			 * @brief Host name used for connecting to the Eclipse Paho MQTT server
			 */
			char * HOST_NAME;

			/**
			 * @brief Port used for connecting to the Eclipse Paho MQTT server
			 */
			int PORT;

			/**
			 * @brief Member attribute for subscribed data
			 */
			RoverMQTT_SubscribeData_t defaultRoverSubscribeData;

			/**
			 * @brief Member attribute for configuration data
			 */
			RoverMQTT_Configure_t defaultRoverMQTTConfigure;

			/**
			 * @brief Member attribute for flags
			 */
			RoverMQTT_StatusFlags_t defaultRoverMQTTFlags;

			/**
			 * @brief Member attribute for return codes
			 */
			RoverMQTT_ReturnCodes_t defaultReturnCodes;

			/**
			 * @brief Client object from MQTTAsync.
			 */
			MQTTAsync client;

			/**
			 * @brief Connect options
			 */
			MQTTAsync_connectOptions conn_opts;

			/**
			 * @brief Disconnect options
			 */
			MQTTAsync_disconnectOptions disc_opts;

			/**
			 * @brief Delivered token
			 */
			volatile MQTTAsync_token deliveredtoken;

		public:
			/**
			 * @brief Destructor for RoverPahoMQTT class.
			 */
			~RoverPahoMQTT ();

			/**
			 * @brief Default constructor for RoverPahoMQTT class
			 */
			RoverPahoMQTT();

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
			 * @param port Port
			 */
			void setPort (const int port);

			/**
			 * @brief Sets payload
			 * @param payload Payload
			 */
			void setPayload (char * payload);

			/**
			 * @brief Sets topic name
			 * @param topic Topic name
			 */
			void setTopic (char * topic);

			/**
			 * @brief Public member function for reading subscribed data. This function requires the RoverPahoMQTT to be subscribed first.
			 */
			char * read (void);

			/**
			 * @brief Public member function checking if the subscribed data is ready. 0-> data ready -1-> data not ready
			 */
			int isDataReady (void);

			/**
			 * @brief Used for publishing to a topic in an MQTT-broker
			 * @return Return code 0-> success; others-> return codes
			 */
			int publish (void);

			/**
			 * @brief Used for subscribing to a topic in an MQTT-broker. To unsubscribe use: RoverPahoMQTT::unsubscribe()
			 * @return Return code 0-> success; others-> return codes
			 */
			int subscribe (void);

			/**
			 * @brief Used for unsubscribing and disconnecting from a subscription to a topic in an MQTT-broker.
			 * @return Return code 0-> success; others-> return codes
			 */
			int unsubscribe (void);


		protected:
			/**
			 * @brief Private member function to flush the connection flags for new operation.
			 * @return void
			 */
			void flushFlags (void);

			/**
			 * @brief Used for constructing the connection address by writing to a string.
			 * @param string String variable to use
			 * @return void
			 */
			void constructAddress (char* string);

			/* Internal callbacks */
			/* Related to publisher */
			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onPublisherConnect in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onPublisherConnect_Redirect (void* context, MQTTAsync_successData* response);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onPublisherSend in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onPublisherSend_Redirect (void* context, MQTTAsync_successData* response);

			/**
			 * @brief Member callback function for onPublisherConnect actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onPublisherConnect (MQTTAsync_successData* response);


			/**
			 * @brief Member callback function for onPublisherSend actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onPublisherSend (MQTTAsync_successData* response);


			/* Related to subscriber */
			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onSubscriberConnect in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onSubscriberConnect_Redirect (void* context, MQTTAsync_successData* response);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onSubscriberMessageArrived in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param topicName received topic name
			 * @param topicLen received topic name length
			 * @param message received message
			 * @return int
			 */
			static int onSubscriberMessageArrived_Redirect (void *context, char *topicName, int topicLen, MQTTAsync_message *message);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onSubscribe in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onSubscribe_Redirect (void* context, MQTTAsync_successData* response);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onSubscribeFailure in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onSubscribeFailure_Redirect (void* context, MQTTAsync_failureData* response);

			/**
			 * @brief Member callback function for onSubscriberConnect actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onSubscriberConnect (MQTTAsync_successData* response);

			/**
			 * @brief Member callback function for onSubscriberMessageArrived actions
			 * @param topicName received topic name
			 * @param topicLen received topic name length
			 * @param message received message
			 * @return int
			 */
			int onSubscriberMessageArrived (char *topicName, int topicLen, MQTTAsync_message *message);

			/**
			 * @brief Member callback function for onSubscribe actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onSubscribe (MQTTAsync_successData* response);

			/**
			 * @brief Member callback function for onSubscribeFailure actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onSubscribeFailure (MQTTAsync_failureData* response);

			/* Related to both */
			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onConnectFailure in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onConnectFailure_Redirect (void* context, MQTTAsync_failureData* response);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onConnectionLost in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param cause Cause of connection loss
			 * @return void
			 */
			static void onConnectionLost_Redirect (void* context, char *cause);

			/**
			 * @brief Member callback function for onConnectFailure actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onConnectFailure (MQTTAsync_failureData* response);

			/**
			 * @brief Static function responsible for calling RoverPahoMQTT::onDisconnect in a given RoverPahoMQTT context.
			 * @param context Given context for RoverPahoMQTT. Internal use: this
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			static void onDisconnect_Redirect (void* context, MQTTAsync_successData* response);

			/**
			 * @brief Member callback function for onConnectionLost actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onConnectionLost (char *cause);

			/**
			 * @brief Member callback function for onDisconnect actions
			 * @param response Response data from MQTTAsync instance
			 * @return void
			 */
			void onDisconnect (MQTTAsync_successData* response);

	};
}



#endif /* ROVERAPI_ROVER_PAHOMQTT_HPP_ */
