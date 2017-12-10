/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTTCommand API - Class for MQTT communication related to rover tasks such as sensor publishing and control data retrieval.
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverMQTTCommand C++API 10.12.2017
 *
 */

#include <roverapi/rover_mqttcommand.hpp>

rover::RoverMQTTCommand::RoverMQTTCommand (char * host, const int port, const int qos, char * clientID)
{
	/* Assign what is constructed */
	this->HOST_NAME = host;
	this->PORT = port;
	this->defaultRoverMQTTConfigure.qos = qos;
	this->defaultRoverMQTTConfigure.clientID = clientID;

	/* Load defaults */
	this->flushFlags();
}

rover::RoverMQTTCommand::~RoverMQTTCommand(){}


