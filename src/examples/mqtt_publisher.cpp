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
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - updated: 17.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//PahoMQTT Include
#include <roverapi/rover_pahomqtt.hpp>

//Your MQTT Broker credentials and info
#define MQTT_BROKER (char*)"127.0.0.1"
#define MQTT_BROKER_PORT 1883  //default:1883
#define ROVER_MQTT_QOS 0 //Quality of service
#define PUBLISH_TOPIC1 (char*)"telemetry"
#define PUBLISH_TOPIC2 (char*)"rover/2/RoverDriving/control"
#define PUBLISH_PAYLOAD (char*)"Hello from rover!"
#define MQTT_USERNAME (char*)"sensor1@DEFAULT_TENANT"
#define MQTT_PASSWORD (char*)"hono-secret"

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started rover mqtt publisher example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    // Configure
    RoverMQTT_Configure_t rover_mqtt_conf;
    rover_mqtt_conf.clientID = (char*)"rover_publisher";        // Identification of the Client
    rover_mqtt_conf.qos      = ROVER_MQTT_QOS;                  // Quality of Service
    rover_mqtt_conf.timeout  = 10000L;                          // Polling timeout, 10000L is fine
    rover_mqtt_conf.topic    = PUBLISH_TOPIC1;                  // Topic name to publish to or subscribe from
    rover_mqtt_conf.username    = MQTT_USERNAME;                // Username - Leave empty for no user and password
    rover_mqtt_conf.password    = MQTT_PASSWORD;                // Password - Leave empty for no user and password
    RoverPahoMQTT rover_mqtt = RoverPahoMQTT (  MQTT_BROKER,    // MQTT-Broker host
                                                MQTT_BROKER_PORT,  // MQTT-Broker port
                                                rover_mqtt_conf);
    // Set payload
    rover_mqtt.setPayload (PUBLISH_PAYLOAD, strlen(PUBLISH_PAYLOAD));

    // Publish is non-blocking, client disconnects afterwards
    if (0 == rover_mqtt.publish())
        printf ("Publishing successful!\n");
    else
        printf ("Publishing unsuccessful!\n");
    
    // Sleep a bit
    r_base.sleep(1000);
    
    // Send the same message a second time, but to a different topic!
    rover_mqtt.setTopic (PUBLISH_TOPIC2);
    if (0 == rover_mqtt.publish())
		printf ("Publishing successful!\n");
	else
		printf ("Publishing unsuccessful!\n");

	printf("Exiting.\n");

	return 0;
}
