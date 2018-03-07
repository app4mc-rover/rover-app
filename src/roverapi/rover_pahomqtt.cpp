/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover PahoMQTT API - Interfaces for Rover PahoMQTT cloud application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created PahoMQTT API 07.12.2017
 *                                          improved PahoMQTT API 10.12.2017
 *
 */

#include <roverapi/rover_pahomqtt.hpp>
#include <cstring>

rover::RoverPahoMQTT::RoverPahoMQTT(){}

rover::RoverPahoMQTT::RoverPahoMQTT (char * host_name, int port, RoverMQTT_Configure_t MQTT_Configure)
{
	this->HOST_NAME = host_name;
	this->PORT = port;
	this->defaultRoverMQTTConfigure = MQTT_Configure;
	this->defaultRoverMQTTConfigure.payload = nullptr;

	if (strlen(MQTT_Configure.username)>1)
	{
		this->defaultRoverMQTTConfigure.username = MQTT_Configure.username;
	}
	else
	{
		this->defaultRoverMQTTConfigure.username = (char*)"";
	}

	if (strlen(MQTT_Configure.password)>1)
	{
		this->defaultRoverMQTTConfigure.password = MQTT_Configure.password;
	}
	else
	{
		this->defaultRoverMQTTConfigure.password = (char*)"";
	}

	/* Load defaults */
	this->flushFlags();

	/* Construct the address */
	this->constructAddress();

	this->createClient();



}

void rover::RoverPahoMQTT::createClient(void) {
	MQTTAsync_create (&(this->client),
                            this->my_address,
                            this->defaultRoverMQTTConfigure.clientID,
                            MQTTCLIENT_PERSISTENCE_NONE,
                            NULL);

	MQTTAsync_setCallbacks(this->client, this, rover::RoverPahoMQTT::onConnectionLost_Redirect,
                                                rover::RoverPahoMQTT::onSubscriberMessageArrived_Redirect,
												NULL);
}

void rover::RoverPahoMQTT::destroyClient(void) {
	MQTTAsync_destroy(&(this->client));
}


rover::RoverPahoMQTT::~RoverPahoMQTT() {
	if (this->defaultRoverMQTTConfigure.payload != nullptr) {
	    delete[] this->defaultRoverMQTTConfigure.payload;
	}

	if (this->defaultRoverSubscribeData.data != nullptr) {
		free(this->defaultRoverSubscribeData.data);
	}

	this->destroyClient();
}

void rover::RoverPahoMQTT::constructAddress (void)
{
	char num_buffer[5] = {'\0'};
	char string[100] = {'\0'};
	sprintf (string, "%s", this->HOST_NAME);
	strcat (string, ":");
	sprintf (num_buffer, "%d", this->PORT);
	strncat(string, num_buffer, strlen(num_buffer));

	num_buffer[0] = 0;
	memcpy(this->my_address,string,strlen(string));
	//printf ("%s\n", this->my_address);
}

void rover::RoverPahoMQTT::flushFlags(void)
{
	this->defaultRoverMQTTFlags.f_mqtt_disconnect_finished = 0;
	this->defaultRoverMQTTFlags.f_mqtt_finished = 0;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 0;
	this->defaultRoverMQTTFlags.f_mqtt_connected = 0;
	this->defaultRoverMQTTFlags.f_mqtt_publish_successful = 0;
	this->deliveredtoken = 0;
	this->defaultRoverSubscribeData.data_ready = 0;
	this->defaultRoverSubscribeData.data = nullptr;
	this->defaultReturnCodes.rc_publish = -1;
	this->defaultReturnCodes.rc_subscribe = -1;
	this->defaultReturnCodes.rc_unsubscribe = -1;
}


int rover::RoverPahoMQTT::connect (void)
{
	int rc = 0;
	const int timeout = 2000;
	int ctr = 0;
	// Only create client if it hasn't been created
	this->flushFlags ();

	this->conn_opts = MQTTAsync_connectOptions_initializer;
	this->conn_opts.keepAliveInterval = 20;
	this->conn_opts.cleansession = 1;
	this->conn_opts.onSuccess = rover::RoverPahoMQTT::onConnect_Redirect;
	this->conn_opts.onFailure = rover::RoverPahoMQTT::onConnectFailure_Redirect;
	this->conn_opts.context = this;

	if (strlen(this->defaultRoverMQTTConfigure.username) > 0)
	{
		this->conn_opts.username = this->defaultRoverMQTTConfigure.username;
	}
	if (strlen(this->defaultRoverMQTTConfigure.password) > 0)
	{
		this->conn_opts.password = this->defaultRoverMQTTConfigure.password;
	}

	if ((rc = MQTTAsync_connect(this->client, &(this->conn_opts))) != MQTTASYNC_SUCCESS)
	{
		//printf("Failed to start connect, return code %d\n", rc);
		this->defaultReturnCodes.rc_publish = rc;
		//return rc;
	}
	else
	{
		printf ("Trying to connect to MQTT broker..\n");
		while (!(this->defaultRoverMQTTFlags.f_mqtt_finished) && !(timeout < ctr))
		{
			ctr = ctr + 1;
			usleep(this->defaultRoverMQTTConfigure.timeout);
		}

		if (timeout < ctr)
		{
			ctr = 0;
			this->defaultReturnCodes.rc_publish = 1; //Failed
		}
	}

	return rc;
}

int rover::RoverPahoMQTT::publish (void)
{
	if (this->defaultRoverMQTTFlags.f_mqtt_connected == 1)
	{
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
		int rc = 0;

		//printf("Successful connection\n");

		opts.onSuccess = rover::RoverPahoMQTT::onPublisherSend_Redirect;
		opts.context = this;

		pubmsg.payload = this->defaultRoverMQTTConfigure.payload;
		pubmsg.payloadlen = strlen(this->defaultRoverMQTTConfigure.payload);
		pubmsg.qos = this->defaultRoverMQTTConfigure.qos;
		pubmsg.retained = 0;
		this->deliveredtoken = 0;

		rc = MQTTAsync_sendMessage(this->client, this->defaultRoverMQTTConfigure.topic, &pubmsg, &opts);
		return rc;
	}
	else
	{
		return -1;
	}

}

int rover::RoverPahoMQTT::unsubscribe (void)
{
	int rc = 0;
	
	this->disc_opts = MQTTAsync_disconnectOptions_initializer;
	this->disc_opts.context = this; //Important to pass context with options.
	this->disc_opts.onSuccess = rover::RoverPahoMQTT::onDisconnect_Redirect;

	if ((rc = MQTTAsync_disconnect(this->client, &(this->disc_opts))) != MQTTASYNC_SUCCESS)
	{
		//printf("Failed to start disconnect, return code %d\n", rc);
		this->defaultReturnCodes.rc_unsubscribe = rc;
		this->defaultRoverMQTTFlags.f_mqtt_subscribed = 0;
		//exit(EXIT_FAILURE);
	}
	else
	{
		while	(!this->defaultRoverMQTTFlags.f_mqtt_disconnect_finished)
			usleep(this->defaultRoverMQTTConfigure.timeout);
	}

	return this->defaultReturnCodes.rc_unsubscribe;
}

int rover::RoverPahoMQTT::subscribe (void)
{
	if (this->defaultRoverMQTTFlags.f_mqtt_connected == 1)
	{
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		int rc = 0;

		//printf("Successful connection\n");

		/*printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
			   "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);*/
		opts.onSuccess = rover::RoverPahoMQTT::onSubscribe_Redirect;
		opts.onFailure = rover::RoverPahoMQTT::onSubscribeFailure_Redirect;
		opts.context = this;

		deliveredtoken = 0;

		rc = MQTTAsync_subscribe(this->client, this->defaultRoverMQTTConfigure.topic, this->defaultRoverMQTTConfigure.qos, &opts);
		return rc;
	}
	else
	{
		return -1;
	}
}

int rover::RoverPahoMQTT::read (char* data)
{
	if (this->defaultRoverSubscribeData.data_ready == 1)
	{
		this->defaultRoverSubscribeData.data_ready = 0;
		sprintf (data, "%s", this->defaultRoverSubscribeData.data);
		return 0;
	}
	else
	{
		sprintf (data, "%s", "N/A");
		return 1;
	}
}

void rover::RoverPahoMQTT::setPort (const int port)
{
	this->PORT = port;
}

void rover::RoverPahoMQTT::setHostName (char * host_name)
{
	this->HOST_NAME = host_name;
}

void rover::RoverPahoMQTT::onConnectionLost (char *cause)
{
//	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
//	int rc = 0;

//	//printf("\nConnection lost\n");
//	//printf("     cause: %s\n", cause);

//	//printf("Reconnecting\n");
//	conn_opts.keepAliveInterval = 20;
//	conn_opts.cleansession = 1;

//	if (strlen(this->defaultRoverMQTTConfigure.username) > 0)
//	{
//		this->conn_opts.username = this->defaultRoverMQTTConfigure.username;
//	}
//	if (strlen(this->defaultRoverMQTTConfigure.password) > 0)
//	{
//		this->conn_opts.password = this->defaultRoverMQTTConfigure.password;
//	}
//	if ((rc = MQTTAsync_connect(this->client, &conn_opts)) != MQTTASYNC_SUCCESS)
//	{
//		//printf("Failed to start connect, return code %d\n", rc);
//		this->defaultReturnCodes.rc_publish = rc;
//		this->defaultReturnCodes.rc_subscribe = rc;
//		this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
//	}
	this->defaultRoverMQTTFlags.f_mqtt_connected = 0;
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
	this->defaultReturnCodes.rc_publish = -1;
	this->defaultReturnCodes.rc_subscribe = -1;

}

void rover::RoverPahoMQTT::onDisconnect (MQTTAsync_successData* response)
{
	//printf("Successful disconnection\n");
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_disconnect_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 0;
	this->defaultReturnCodes.rc_unsubscribe = 0;
	this->defaultRoverMQTTFlags.f_mqtt_connected = 0;
}


void rover::RoverPahoMQTT::onConnect (MQTTAsync_successData* response)
{
	this->defaultRoverMQTTFlags.f_mqtt_connected = 1;
}

void rover::RoverPahoMQTT::onPublisherSend (MQTTAsync_successData* response)
{
//	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
//	int rc = 0;

//	//printf("Message with token value %d delivery confirmed\n", response->token);

//	opts.onSuccess = rover::RoverPahoMQTT::onDisconnect_Redirect;
//	opts.context = this;

//	if ((rc = MQTTAsync_disconnect(this->client, &opts)) != MQTTASYNC_SUCCESS)
//	{
//		//printf("Failed to start sendMessage, return code %d\n", rc);
//		this->defaultReturnCodes.rc_publish = rc;
//		//exit(EXIT_FAILURE);
//	}
//	else
//	{
//		this->defaultReturnCodes.rc_publish = 0;
//		this->defaultRoverMQTTFlags.f_mqtt_publish_successful = 1;
//	}
}

void rover::RoverPahoMQTT::onConnectFailure (MQTTAsync_failureData* response)
{
	//printf("Connect failed, rc %d\n", response ? response->code : 0);
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_connected = 0;
}


int rover::RoverPahoMQTT::onSubscriberMessageArrived (char *topicName, int topicLen, MQTTAsync_message *message)
{
	int i = 0;
	char* payloadptr = {};
	char* buf = {};

	if (this->defaultRoverSubscribeData.data_ready == 0)
	{
		buf = (char *) malloc (sizeof(char *) * message->payloadlen + 1);
		// If memory couldn't be allocated
		if (!buf) {
			return 0;
		}

		payloadptr = (char *) message->payload;
		for(i=0; i<message->payloadlen; i++)
		{
			buf[i] = *payloadptr;
			payloadptr++;
		}
		buf[message->payloadlen] = 0;

		// Delete pre-existing data
		if (this->defaultRoverSubscribeData.data)
		{
			free(this->defaultRoverSubscribeData.data);
		}

		this->defaultRoverSubscribeData.data = buf;
		//printf ("%s\n",buf);
		//free(buf);

		MQTTAsync_freeMessage (&message);
		MQTTAsync_free (topicName);

		this->defaultRoverSubscribeData.data_ready = 1;
	}
	return 1;
}

void rover::RoverPahoMQTT::onSubscribeFailure (MQTTAsync_failureData* response)
{
	//printf("Subscribe failed, rc %d\n", response ? response->code : 0);
	this->defaultReturnCodes.rc_subscribe = response ? response->code : 0;
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 1; //I know this sounds wrong, but this is OK!
}

void rover::RoverPahoMQTT::onSubscribe (MQTTAsync_successData* response)
{
	//printf("Subscribe succeeded\n");
	this->defaultReturnCodes.rc_subscribe = 0;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 1;
}

void rover::RoverPahoMQTT::setPayload (const char * payload, int payloadLen)
{
	if (this->defaultRoverMQTTConfigure.payload != nullptr) {
	    delete[] this->defaultRoverMQTTConfigure.payload;
	    this->defaultRoverMQTTConfigure.payload = nullptr;
	}

	this->defaultRoverMQTTConfigure.payload = new char [payloadLen+1];
	std::strcpy (this->defaultRoverMQTTConfigure.payload, payload);
}

void rover::RoverPahoMQTT::setTopic (char * topic)
{
	this->defaultRoverMQTTConfigure.topic = topic;
}

int rover::RoverPahoMQTT::isDataReady (void)
{
	if (this->defaultRoverSubscribeData.data_ready == 1)
	{
		return 0;
	}
	else
		return -1;
}

int rover::RoverPahoMQTT::getConnected(void)
{
	return this->defaultRoverMQTTFlags.f_mqtt_connected;
}

void rover::RoverPahoMQTT::onConnect_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onConnect(response);
}

void rover::RoverPahoMQTT::onPublisherSend_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onPublisherSend(response);
}

int rover::RoverPahoMQTT::onSubscriberMessageArrived_Redirect (void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	return m->onSubscriberMessageArrived (topicName, topicLen, message);
}

void rover::RoverPahoMQTT::onSubscribe_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onSubscribe (response);
}

void rover::RoverPahoMQTT::onSubscribeFailure_Redirect (void* context, MQTTAsync_failureData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onSubscribeFailure (response);
}

void rover::RoverPahoMQTT::onConnectFailure_Redirect (void* context, MQTTAsync_failureData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onConnectFailure (response);
}

void rover::RoverPahoMQTT::onConnectionLost_Redirect (void* context, char* cause)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onConnectionLost (cause);
}

void rover::RoverPahoMQTT::onDisconnect_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onDisconnect (response);
}
