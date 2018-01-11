/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTT telemetry control example
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//MQTTCommand Include
#include <roverapi/rover_mqttcommand.hpp>

//Sensor APIs Include
#include <roverapi/rover_grooveultrasonic.hpp>
#include <roverapi/rover_gy521.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_hmc5883l.hpp>
#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/rover_qmc5883l.hpp>
#include <roverapi/rover_dht22.hpp>

//Your MQTT Broker credentials
#define MQTT_BROKER "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define ROVER_IDENTITY 1 //Rover ID
#define ROVER_MQTT_QOS 1 //Quality of service

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started roverapp mqtt_telemetry example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    //MQTT Publisher
    RoverMQTTCommand rover_mqtt_publisher = RoverMQTTCommand (	MQTT_BROKER,
														MQTT_BROKER_PORT,
														ROVER_IDENTITY,
														ROVER_MQTT_QOS,
														"rover_mqtt_publisher");
	
	//MQTT Subscriber
    RoverMQTTCommand rover_mqtt_subscriber = RoverMQTTCommand (	MQTT_BROKER,
														MQTT_BROKER_PORT,
														ROVER_IDENTITY,
														ROVER_MQTT_QOS,
														"rover_mqtt_subscriber");
														
														
	//TODO: Write a single thread telemetry controller!

	printf("Exiting.\n");

	return 0;
}