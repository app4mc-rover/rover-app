/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTT Subscriber example
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
#define MQTT_BROKER_PORT 1887 // default:1883
#define ROVER_MQTT_QOS 1 //Quality of service
#define SUBSCRIBE_TOPIC "rover/1/RoverDriving/control"
#define RECEIVE_PAYLOAD_BUFSIZE 50 // <-- Here we want to receive up to 50 characters of data
                                   // This should be modified in case you want to receive more!

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started rover mqtt subscriber example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    char data [RECEIVE_PAYLOAD_BUFSIZE];
    int8_t second = 0;
    
    // Subscribe
    RoverMQTT_Configure_t rover_mqtt_conf;
    rover_mqtt_conf.clientID = "rover_subscriber";               // Identification of the Client
    rover_mqtt_conf.payload  = "nothing";                       // Message to send
    rover_mqtt_conf.qos      = ROVER_MQTT_QOS;                  // Quality of Service
    rover_mqtt_conf.timeout  = 10000L;                          // Polling timeout, 10000L is fine
    rover_mqtt_conf.topic    = SUBSCRIBE_TOPIC;                 // Topic name to subscribe to
    RoverPahoMQTT rover_mqtt = RoverPahoMQTT (  MQTT_BROKER,      // MQTT-Broker host
                                                MQTT_BROKER_PORT, // MQTT-Broker port
                                                rover_mqtt_conf);
                                                
    // Subscribe is non-blocking, works with internal callbacks
    if (0 == rover_mqtt.subscribe())
    {
        printf ("Subscribe successful!\n");

        // Try to read any coming data for roughly half a minute
        printf ("Reading data for half a minute from the topic %s\n", SUBSCRIBE_TOPIC);
		while (second < 30)
		{
			if (0 == rover_mqtt.read(data)) // Read function returns the latest received data from subscribed topic
			{
				printf ("Received data=%s\n", data);
			}
			else
			{
				printf ("No available data\n");
			}

			second += 1;
			r_base.sleep(1000);
		}

		printf ("Done..\n");

    }
    else
    {
        printf ("Subscribe unsuccessful!\n");
    }
    


	printf("Exiting.\n");

	return 0;
}
