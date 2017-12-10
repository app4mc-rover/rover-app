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
#include <cstring>
/* json-cpp library */
#if SIMULATOR
#include <jsoncpp/json/json.h>
#else
#include <json/json.h>
#endif

rover::RoverMQTTCommand::RoverMQTTCommand (char * host, const int port, const int qos, char * clientID)
{
	/* Assign what is constructed */
	this->HOST_NAME = host;
	this->PORT = port;
	this->defaultRoverMQTTConfigure.qos = qos;
	this->defaultRoverMQTTConfigure.clientID = clientID;
	this->defaultRoverMQTTConfigure.timeout = 10000L;

	/* Load defaults */
	this->flushFlags();
}

rover::RoverMQTTCommand::~RoverMQTTCommand(){}

int rover::RoverMQTTCommand::publishToSensorTopic (RoverSensorData_t sensor_data)
{
	/* Set topic name */
	this->setTopic(sensorTopic);

	/* Construct payload from sensor_data */
	Json::Value data;
	data["dht22"]["temperature"] = sensor_data.temperature;
	data["dht22"]["humidity"] = sensor_data.humidity;
	data["infrared"]["0"] = sensor_data.infrared[0];
	data["infrared"]["1"] = sensor_data.infrared[1];
	data["infrared"]["2"] = sensor_data.infrared[2];
	data["infrared"]["3"] = sensor_data.infrared[3];
	data["ultrasonic"]["front"] = sensor_data.ultrasonic_front;
	data["ultrasonic"]["rear"] = sensor_data.ultrasonic_rear;
	data["hmc5883l"]["bearing"] = sensor_data.hmc5883l_bearing;
	data["qmc5883l"]["bearing"] = sensor_data.qmc5883l_bearing;
	data["gy521"]["bearing"] = sensor_data.gy521_bearing;
	data["gy521"]["gyro"]["x"] = sensor_data.gy521_gyro_x;
	data["gy521"]["gyro"]["y"] = sensor_data.gy521_gyro_y;
	data["gy521"]["gyro"]["z"] = sensor_data.gy521_gyro_z;
	data["gy521"]["angle"]["x"] = sensor_data.gy521_angle_x;
	data["gy521"]["angle"]["y"] = sensor_data.gy521_angle_y;
	data["gy521"]["angle"]["z"] = sensor_data.gy521_angle_z;
	data["gy521"]["accel"]["x"] = sensor_data.gy521_accel_x;
	data["gy521"]["accel"]["y"] = sensor_data.gy521_accel_y;
	data["gy521"]["accel"]["z"] = sensor_data.gy521_accel_z;

	/* Convert JSON data to string */
	Json::FastWriter  string_writer;
	std::string temp = string_writer.write(data);

	/* Convert from string to char* */
	char * temp2 = new char [temp.length()+1];
	std::strcpy (temp2, temp.c_str());

	/* Set payload to constructed char* */
	this->setPayload(temp2);

	/* Call publish */
	return this->publish();
}
