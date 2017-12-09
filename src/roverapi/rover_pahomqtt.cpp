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
 *
 */

#include <roverapi/rover_pahomqtt.hpp>

/* Flags */
static int mqtt_disconnect_finished = 0;
static int mqtt_subscribed = 0;
static int mqtt_finished = 0;

rover::RoverMQTT_Configure_t default_MQTT_configure;

volatile MQTTAsync_token deliveredtoken;

rover::RoverPahoMQTT::RoverPahoMQTT (char * host_name, int port, RoverMQTT_Configure_t MQTT_Configure)
{
	this->HOST_NAME = host_name;
	this->PORT = port;
	this->rover_MQTT_configure = MQTT_Configure;
	default_MQTT_configure = MQTT_Configure;
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
	printf ("address: %s", string);
}

int rover::RoverPahoMQTT::publish (void)
{
	this->conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	char my_addr[20];
	this->constructAddress (my_addr);

	MQTTAsync_create (	&(this->client),
						my_addr,
						this->rover_MQTT_configure.clientID,
						MQTTCLIENT_PERSISTENCE_NONE,
						NULL);

	MQTTAsync_setCallbacks(this->client, NULL, onConnectionLost, NULL, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onPublisherConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = this->client;

	if ((rc = MQTTAsync_connect(this->client, &(this->conn_opts))) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		return rc;
	}

	/*printf("Waiting for publication of %s\n"
		 "on topic %s for client with ClientID: %s\n",
		 PAYLOAD, TOPIC, CLIENTID);*/
	while (!mqtt_finished)
		usleep(this->rover_MQTT_configure.timeout);

	MQTTAsync_destroy(&client);
	return rc;
}

int rover::RoverPahoMQTT::subscribe (void)
{
	this->conn_opts = MQTTAsync_connectOptions_initializer;
	this->disc_opts = MQTTAsync_disconnectOptions_initializer;
	int rc;
	int ch;

	char my_addr[20];
	this->constructAddress (my_addr);

	MQTTAsync_create (	&(this->client),
							my_addr,
							this->rover_MQTT_configure.clientID,
							MQTTCLIENT_PERSISTENCE_NONE,
							NULL);

	MQTTAsync_setCallbacks(this->client, this->client, onConnectionLost, onSubscriberMessageArrived, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onSubscriberConnect;
	conn_opts.onFailure = onSubscribeFailure;
	conn_opts.context = client;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	while	(!mqtt_subscribed)
		usleep(this->rover_MQTT_configure.timeout);

	if (mqtt_finished)
		goto exit;

	do
	{
		ch = getchar();
	} while (ch!='Q' && ch != 'q');

	disc_opts.onSuccess = onDisconnect;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
 	while	(!mqtt_disconnect_finished)
		usleep(this->rover_MQTT_configure.timeout);

exit:
	MQTTAsync_destroy(&client);
	return rc;
}

void rover::RoverPahoMQTT::setPort (const int port)
{
	this->PORT = port;
}

void rover::RoverPahoMQTT::setHostName (char * host_name)
{
	this->HOST_NAME = host_name;
}

void rover::RoverPahoMQTT::onConnectionLost (void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		mqtt_disconnect_finished = 1;
	}
}

void rover::RoverPahoMQTT::onDisconnect (void* context, MQTTAsync_successData* response)
{
	printf("Successful disconnection\n");
	mqtt_finished = 1;
}

void rover::RoverPahoMQTT::onPublisherSend (void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message with token value %d delivery confirmed\n", response->token);

	opts.onSuccess = onDisconnect;
	opts.context = client;

	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void rover::RoverPahoMQTT::onConnectFailure (void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response ? response->code : 0);
	mqtt_finished = 1;
}

void rover::RoverPahoMQTT::onPublisherConnect (void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	printf("Successful connection\n");

	opts.onSuccess = onPublisherSend;
	opts.context = client;

	pubmsg.payload = &default_MQTT_configure.payload;
	pubmsg.payloadlen = strlen(default_MQTT_configure.payload);
	pubmsg.qos = default_MQTT_configure.qos;
	pubmsg.retained = 0;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(client, default_MQTT_configure.topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void rover::RoverPahoMQTT::onSubscriberConnect(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	printf("Successful connection\n");

	/*printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);*/
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	deliveredtoken = 0;

	if ((rc = MQTTAsync_subscribe(client, default_MQTT_configure.topic, default_MQTT_configure.qos, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

int rover::RoverPahoMQTT::onSubscriberMessageArrived (void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
	int i;
	char* payloadptr;

	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");

	payloadptr = (char *) message->payload;
	for(i=0; i<message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void rover::RoverPahoMQTT::onSubscribeFailure (void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response ? response->code : 0);
	mqtt_finished = 1;
}

void rover::RoverPahoMQTT::onSubscribe (void* context, MQTTAsync_successData* response)
{
	printf("Subscribe succeeded\n");
	mqtt_subscribed = 1;
}

void rover::RoverPahoMQTT::setPayload (char * payload)
{
	this->rover_MQTT_configure.payload = payload;
	default_MQTT_configure.payload = payload;
}

void rover::RoverPahoMQTT::setTopic (char * topic)
{
	this->rover_MQTT_configure.topic = topic;
	default_MQTT_configure.topic = topic;
}

