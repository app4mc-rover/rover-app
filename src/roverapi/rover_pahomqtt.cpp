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

rover::RoverPahoMQTT::RoverPahoMQTT(){}

rover::RoverPahoMQTT::RoverPahoMQTT (char * host_name, int port, RoverMQTT_Configure_t MQTT_Configure)
{
	this->HOST_NAME = host_name;
	this->PORT = port;
	this->defaultRoverMQTTConfigure = MQTT_Configure;

	/* Load defaults */
	this->flushFlags();
}

rover::RoverPahoMQTT::~RoverPahoMQTT() {}

void rover::RoverPahoMQTT::constructAddress (char* string)
{
	char num_buffer[5];
	sprintf (string, this->HOST_NAME);
	strcat (string, ":");
	snprintf (num_buffer, sizeof(num_buffer), "%d", this->PORT);
	strcat(string, num_buffer);
	num_buffer[0] = 0;
}

void rover::RoverPahoMQTT::flushFlags(void)
{
	this->defaultRoverMQTTFlags.f_mqtt_disconnect_finished = 0;
	this->defaultRoverMQTTFlags.f_mqtt_finished = 0;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 0;
	this->defaultRoverMQTTFlags.f_mqtt_publish_successful = 0;
	this->deliveredtoken = 0;
	this->defaultRoverSubscribeData.data_ready = 0;
	this->defaultRoverSubscribeData.data = "N/A";
	this->defaultReturnCodes.rc_publish = -1;
	this->defaultReturnCodes.rc_subscribe = -1;
	this->defaultReturnCodes.rc_unsubscribe = -1;
}

int rover::RoverPahoMQTT::publish (void)
{
	this->flushFlags ();
	this->client = new MQTTAsync;

	this->conn_opts = MQTTAsync_connectOptions_initializer;

	int rc;

	/* Construct the address */
	char my_addr[20];
	this->constructAddress (my_addr);

	MQTTAsync_create (	&(this->client),
						my_addr,
						this->defaultRoverMQTTConfigure.clientID,
						MQTTCLIENT_PERSISTENCE_NONE,
						NULL);

	MQTTAsync_setCallbacks (this->client, NULL, rover::RoverPahoMQTT::onConnectionLost_Redirect, NULL, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = rover::RoverPahoMQTT::onPublisherConnect_Redirect;
	conn_opts.onFailure = rover::RoverPahoMQTT::onConnectFailure_Redirect;
	conn_opts.context = this;

	if ((rc = MQTTAsync_connect(this->client, &(this->conn_opts))) != MQTTASYNC_SUCCESS)
	{
		//printf("Failed to start connect, return code %d\n", rc);
		this->defaultReturnCodes.rc_publish = rc;
		//return rc;
	}
	else
	{
		while (!(this->defaultRoverMQTTFlags.f_mqtt_finished))
			usleep(this->defaultRoverMQTTConfigure.timeout);
	}

	MQTTAsync_destroy(&(this->client));

	return this->defaultReturnCodes.rc_publish;
}

int rover::RoverPahoMQTT::unsubscribe (void)
{
	int rc;

	disc_opts.onSuccess = rover::RoverPahoMQTT::onDisconnect_Redirect;

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
	this->flushFlags ();
	this->client = new MQTTAsync;

	this->conn_opts = MQTTAsync_connectOptions_initializer;
	this->disc_opts = MQTTAsync_disconnectOptions_initializer;

	this->disc_opts.context = this; //Important to pass context with options.

	int rc;
	int ch;

	/* Construct the address */
	char my_addr[20];
	this->constructAddress (my_addr);

	MQTTAsync_create (	&(this->client),
							my_addr,
							this->defaultRoverMQTTConfigure.clientID,
							MQTTCLIENT_PERSISTENCE_NONE,
							NULL);

	MQTTAsync_setCallbacks(this->client, this, 	rover::RoverPahoMQTT::onConnectionLost_Redirect,
												rover::RoverPahoMQTT::onSubscriberMessageArrived_Redirect,
												NULL);

	this->conn_opts.keepAliveInterval = 20;
	this->conn_opts.cleansession = 1;
	this->conn_opts.onSuccess = rover::RoverPahoMQTT::onSubscriberConnect_Redirect;
	this->conn_opts.onFailure = rover::RoverPahoMQTT::onSubscribeFailure_Redirect;
	this->conn_opts.context = this;

	if ((rc = MQTTAsync_connect(this->client, &(this->conn_opts))) != MQTTASYNC_SUCCESS)
	{
		//printf("Failed to start connect, return code %d\n", rc);
		this->defaultReturnCodes.rc_subscribe = rc;
	}
	else
	{
		while (!this->defaultRoverMQTTFlags.f_mqtt_subscribed)
		{
			usleep(this->defaultRoverMQTTConfigure.timeout);
		}
	}

	if (this->defaultRoverMQTTFlags.f_mqtt_finished)
	{
		MQTTAsync_destroy(&(this->client));
	}

	return this->defaultReturnCodes.rc_subscribe;
}

void rover::RoverPahoMQTT::read (char* data)
{
	if (this->defaultRoverSubscribeData.data_ready == 1)
	{
		this->defaultRoverSubscribeData.data_ready = 0;
		sprintf (data, this->defaultRoverSubscribeData.data);
	}
	else
		sprintf (data, "N/A");
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
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	//printf("\nConnection lost\n");
	//printf("     cause: %s\n", cause);

	//printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(this->client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		this->defaultReturnCodes.rc_publish = rc;
		this->defaultReturnCodes.rc_subscribe = rc;
	}
}

void rover::RoverPahoMQTT::onDisconnect (MQTTAsync_successData* response)
{
	//printf("Successful disconnection\n");
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_disconnect_finished = 1;
	this->defaultRoverMQTTFlags.f_mqtt_subscribed = 0;
	this->defaultReturnCodes.rc_unsubscribe = 0;
}

void rover::RoverPahoMQTT::onPublisherSend (MQTTAsync_successData* response)
{
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	//printf("Message with token value %d delivery confirmed\n", response->token);

	opts.onSuccess = rover::RoverPahoMQTT::onDisconnect_Redirect;
	opts.context = this;

	if ((rc = MQTTAsync_disconnect(this->client, &opts)) != MQTTASYNC_SUCCESS)
	{
		//printf("Failed to start sendMessage, return code %d\n", rc);
		this->defaultReturnCodes.rc_publish = rc;
		//exit(EXIT_FAILURE);
	}
	else
	{
		this->defaultReturnCodes.rc_publish = 0;
		this->defaultRoverMQTTFlags.f_mqtt_publish_successful = 1;
	}

}

void rover::RoverPahoMQTT::onConnectFailure (MQTTAsync_failureData* response)
{
	//printf("Connect failed, rc %d\n", response ? response->code : 0);
	this->defaultRoverMQTTFlags.f_mqtt_finished = 1;
}

void rover::RoverPahoMQTT::onSubscriberConnect (MQTTAsync_successData* response)
{
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	//printf("Successful connection\n");

	/*printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);*/
	opts.onSuccess = rover::RoverPahoMQTT::onSubscribe_Redirect;
	opts.onFailure = rover::RoverPahoMQTT::onSubscribeFailure_Redirect;
	opts.context = this;

	deliveredtoken = 0;

	if ((rc = MQTTAsync_subscribe(this->client, this->defaultRoverMQTTConfigure.topic, this->defaultRoverMQTTConfigure.qos, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf ("Failed to start subscribe, return code %d\n", rc);
		this->defaultReturnCodes.rc_subscribe = rc;
		exit (EXIT_FAILURE);
	}
}

int rover::RoverPahoMQTT::onSubscriberMessageArrived (char *topicName, int topicLen, MQTTAsync_message *message)
{
	int i;
	char* payloadptr;
	char* buf;

	if (this -> defaultRoverSubscribeData.data_ready == 0)
	{
		buf = (char *) malloc (sizeof(char *) * message->payloadlen + 1);

		payloadptr = (char *) message->payload;
		for(i=0; i<message->payloadlen; i++)
		{
			buf[i] = *payloadptr;
			payloadptr++;
		}
		buf[message->payloadlen] = 0;

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

void rover::RoverPahoMQTT::setPayload (char * payload)
{
	this->defaultRoverMQTTConfigure.payload = payload;
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

void rover::RoverPahoMQTT::onPublisherConnect (MQTTAsync_successData* response)
{
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	//printf("Successful connection\n");

	opts.onSuccess = rover::RoverPahoMQTT::onPublisherSend_Redirect;
	opts.context = this;

	pubmsg.payload = this->defaultRoverMQTTConfigure.payload;
	pubmsg.payloadlen = strlen(this->defaultRoverMQTTConfigure.payload);
	pubmsg.qos = this->defaultRoverMQTTConfigure.qos;
	pubmsg.retained = 0;
	this->deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(this->client, this->defaultRoverMQTTConfigure.topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		this->defaultReturnCodes.rc_publish = rc;
		//printf("Failed to start sendMessage, return code %d\n", rc);
		//exit(EXIT_FAILURE);
	}
}

void rover::RoverPahoMQTT::onPublisherConnect_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onPublisherConnect(response);
}

void rover::RoverPahoMQTT::onPublisherSend_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onPublisherSend(response);
}

void rover::RoverPahoMQTT::onSubscriberConnect_Redirect (void* context, MQTTAsync_successData* response)
{
	rover::RoverPahoMQTT* m = (rover::RoverPahoMQTT*) context;
	m->onSubscriberConnect(response);
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
