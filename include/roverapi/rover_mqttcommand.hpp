/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover MQTTCommand API - Class for MQTT communication related to rover tasks such as sensor publishing and control data retrieval.
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverMQTTCommand C++API 10.12.2017
 *
 */

#ifndef ROVERAPI_ROVER_MQTTCOMMAND_HPP_
#define ROVERAPI_ROVER_MQTTCOMMAND_HPP_

#include <roverapi/rover_pahomqtt.hpp>

#define drivingTopic  "rover/RoverDriving/control"
#define sensorTopic   "rover/RoverSensor/sensors"

namespace rover
{
	/**
	 * @brief Data type to store rover's sensor data
	 */
	typedef struct
	{
		float temperature;
		float humidity;
		float infrared[4];
		float ultrasonic_front;
		float ultrasonic_rear;
		float hmc5883l_bearing;
		float qmc5883l_bearing;
		float gy521_bearing;
		int8_t gy521_gyro_x;
		int8_t gy521_gyro_y;
		int8_t gy521_gyro_z;
		float gy521_angle_x;
		float gy521_angle_y;
		float gy521_angle_z;
		int8_t gy521_accel_x;
		int8_t gy521_accel_y;
		int8_t gy521_accel_z;
	} RoverSensorData_t;

	/**
	 * @brief Data type to indicate rover's driving mode
	 */
	enum RoverDrivingMode_t {NONE_ = 0, MANUAL_ = 0, ACC_, PARKING_LEFT_, PARKING_RIGHT_, BOOTH_MODE_1_, BOOTH_MODE_2_};

	/**
	 * @brief Data type to store rover's control data
	 */
	typedef struct
	{
		RoverDrivingMode_t driving_mode;
		int speed;
		char command;
	} RoverControlData_t;

	/**
	 * @brief RoverMQTTCommand class is an implementation class extending RoverPahoMQTT for rover-specific topic subscription and publishing using JSON for parsing and using
	 * predefined type variables such as RoverControlData_t and RoverSensorData_t.
	 */
	class RoverMQTTCommand: private RoverPahoMQTT
	{
		public:
			/**
			 * @brief Copy constructor for RoverMQTTCommand class
			 */
			RoverMQTTCommand (char * host, const int port, const int qos, char * clientID);

			/**
			 * @brief Destructor for RoverMQTTCommand class
			 */
			~RoverMQTTCommand ();

			/**
			 * @brief Publishes a message to rover's sensor topic
			 * @param sensor_data Sensor data to be used in the message
			 * @return int Return code
			 */
			int publishToSensorTopic (RoverSensorData_t sensor_data);

			/**
			 * @brief Subscribes to rover's driving topic
			 */
			int subscribeToDrivingTopic ();

			/**
			 * @brief Unsubscribes to rover's driving topic
			 */
			int unsubscribeToDrivingTopic ();

			/**
			 * @brief Returns the retrieved driving information
			 */
			RoverControlData_t readFromDrivingTopic ();

	};
}

#endif /* ROVERAPI_ROVER_MQTTCOMMAND_HPP_ */
