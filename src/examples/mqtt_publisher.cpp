/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTT Publisher example
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//PahoMQTT Include
#include <roverapi/rover_pahomqtt.hpp>

//Your MQTT Broker credentials and info
#define MQTT_BROKER "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define ROVER_IDENTITY 1 //Rover ID
#define ROVER_MQTT_QOS 1 //Quality of service
#define PUBLISH_TOPIC1 "rover/1/RoverDriving/control"
#define PUBLISH_TOPIC2 "rover/2/RoverDriving/control"
#define PUBLISH_PAYLOAD "Hello from rover!"

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started rover mqtt publisher example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    // Publish to a topic in an MQTTv3 or MQTTv3.1 broker using RoverPahoMQTT class
    RoverMQTT_Configure_t rover_mqtt_conf;
    rover_mqtt_conf.clientID = "rover_publisher";               // Identification of the Client
    rover_mqtt_conf.payload  = PUBLISH_PAYLOAD;                 // Message to send
    rover_mqtt_conf.qos      = ROVER_MQTT_QOS;                               // Quality of Service
    rover_mqtt_conf.timeout  = 10000L;                          // Polling timeout, 10000L is fine
    rover_mqtt_conf.topic    = PUBLISH_TOPIC1;  // Topic name to publish to or subscribe from
    RoverPahoMQTT rover_mqtt = RoverPahoMQTT (  MQTT_BROKER,    // MQTT-Broker host
                                                MQTT_BROKER_PORT,           // MQTT-Broker port
                                                rover_mqtt_conf);
    // Overriding payload and topic
    char payloadMsg[] = "Hi from rover!";
    rover_mqtt.setPayload (payloadMsg, strlen(payloadMsg));
    rover_mqtt.setTopic (PUBLISH_TOPIC2);
    // Publish is non-blocking, client disconnects afterwards
    if (0 == rover_mqtt.publish())
        printf ("Publishing successful!\n");
    else
        printf ("Publishing unsuccessful!\n");
    
    // Sleep a bit
    r_base.sleep(5000);
    
	printf("Exiting.\n");

	return 0;
}
