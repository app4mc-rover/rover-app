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

rover::RoverMQTTCommand::RoverMQTTCommand (char * host, const int port, const int roverID, const int qos, char * clientID)
{
	/* Assign what is constructed */
	this->HOST_NAME = host;
	this->PORT = port;
	if (roverID < 1 || roverID > 99)
	{
		printf ("Invalid roverID in RoverMQTTCommand: It should be 1 to 99!\n");
		this->ROVER_ID = 99;
	}
	else
	{
		this->ROVER_ID = roverID;
	}
	this->defaultRoverMQTTConfigure.qos = qos;
	this->defaultRoverMQTTConfigure.clientID = clientID;
	this->defaultRoverMQTTConfigure.timeout = 10000L;

	/* Load defaults */
	this->flushFlags();
}

rover::RoverMQTTCommand::~RoverMQTTCommand(){}

int rover::RoverMQTTCommand::publishToSensorTopic (RoverSensorData_t sensor_data)
{
	/* Common buffer to use */
	char topicBuffer_RoverMQTTCommand[64];
	char numBuffer_RoverMQTTCommand[2];

	/* Set topic name */
	/* Add inital part of the topic name */
	sprintf(topicBuffer_RoverMQTTCommand, topicPrefix);

	/* Concatanate rover ID */
	snprintf(numBuffer_RoverMQTTCommand, sizeof(numBuffer_RoverMQTTCommand), "%d", this->ROVER_ID);
	strcat(topicBuffer_RoverMQTTCommand, numBuffer_RoverMQTTCommand);
	numBuffer_RoverMQTTCommand[0] = 0; //Clear array

	/* Add the rest of the topic name */
	strcat(topicBuffer_RoverMQTTCommand, sensorSubTopic);
	this->setTopic (topicBuffer_RoverMQTTCommand);

	/* Construct payload from sensor_data */
	Json::Value data;
	data["dht22"]["temperature"] = sensor_data.temperature;
	data["dht22"]["humidity"] = sensor_data.humidity;
	data["infrared"]["rear-right"] = sensor_data.infrared[0];
	data["infrared"]["rear-left"] = sensor_data.infrared[1];
	data["infrared"]["front-right"] = sensor_data.infrared[2];
	data["infrared"]["front-left"] = sensor_data.infrared[3];
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

int rover::RoverMQTTCommand::subscribeToDrivingTopic (void)
{
	/* Set topic */
	/* Common buffer to use */
	char topicBuffer_RoverMQTTCommand[64];
	char numBuffer_RoverMQTTCommand[2];

	/* Set topic name */
	/* Add inital part of the topic name */
	sprintf(topicBuffer_RoverMQTTCommand, topicPrefix);

	/* Concatanate rover ID */
	snprintf(numBuffer_RoverMQTTCommand, sizeof(numBuffer_RoverMQTTCommand), "%d", this->ROVER_ID);
	strcat(topicBuffer_RoverMQTTCommand, numBuffer_RoverMQTTCommand);
	numBuffer_RoverMQTTCommand[0] = 0; //Clear array

	/* Add the rest of the topic name */
	strcat(topicBuffer_RoverMQTTCommand, drivingSubTopic);
	this->setTopic (topicBuffer_RoverMQTTCommand);

	/* Subscribe */
	return this->subscribe();
}

int rover::RoverMQTTCommand::unsubscribeToDrivingTopic (void)
{
	/* Unsubscribe */
	return this->unsubscribe();
}

rover::RoverControlData_t rover::RoverMQTTCommand::readFromDrivingTopic (void)
{
	char data[MQTT_BUFSIZE];
	RoverControlData_t control_data;
	bool parsingSuccessful = false;

	/* Read the subscribed data into data array */
	this->read(data);
	//printf("data=%s\n",data);

	/* Check if data received is not valid */
	if (data[0]=='N' && data[1]=='/' && data[2]=='A')
	{
		control_data.data_ready = 0;
	}
	else
	{
		/* Data is ready */
		control_data.data_ready = 1;

		/* Variables to use in parsing */
		Json::Value root;
		Json::Reader reader;

		/* Parse the data and construct control data*/
		try
		{
			parsingSuccessful = reader.parse( data, root );
			if ( !parsingSuccessful )
			{
				fprintf (stderr, "Failed to parse data in RoverMQTTCommand\n");
			}
			else
			{
				try
				{
					control_data.speed = root["speed"].asInt();
					control_data.driving_mode = (rover::RoverDrivingMode_t) root["mode"].asInt();
					control_data.command = root["command"].asString()[0];
				}
				catch (int exception_nr)
				{
					fprintf (stderr, "An exception with Nr. %d occurred while parsing the data in RoverMQTTCommand\n", exception_nr);
					parsingSuccessful = false;
				}

			}
		}
		catch (int exception_nr)
		{
			fprintf (stderr, "An exception with Nr. %d occurred while parsing the data in RoverMQTTCommand\n", exception_nr);
			parsingSuccessful = false;
		}

		if (!parsingSuccessful)
		{
			control_data.data_ready = 0;
			control_data.speed = 0;
			control_data.command = 'F';
			control_data.driving_mode = NONE_;
		}
	}

	/* Return control data */
	return control_data;
}

char * rover::RoverMQTTCommand::getTopicName (void)
{
	return this->defaultRoverMQTTConfigure.topic;
}

int rover::RoverMQTTCommand::getRoverID (void)
{
	return this->ROVER_ID;
}

void rover::RoverMQTTCommand::setRoverID (const int rover_id)
{
	this->ROVER_ID = rover_id;
}

