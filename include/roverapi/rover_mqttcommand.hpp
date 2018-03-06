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

/* Defines regarding topic naming */
// Topic naming for control such as the following:
//  rover/<roverID 1-99>/RoverDriving/control
//  e.g rover/1/RoverDriving/Control
// Topic naming for sensor data and core data is : telemetry
#define topicPrefix "rover/"
#define drivingSubTopic "/RoverDriving/control"
#define telemetrySubTopic "/telemetry"
#define telemetryTopic "telemetry"

/* Buffer size for received message for MQTT */
#define MQTT_BUFSIZE 256

namespace rover
{
	/**
	 * @brief Data type to store rover's sensor data
	 */
	typedef struct
	{
		float infrared[4];
		float ultrasonic_front;
		float ultrasonic_rear;
		float hmc5883l_bearing;
		int8_t gy521_gyro_x;
		int8_t gy521_gyro_y;
		int8_t gy521_gyro_z;
		float gy521_angle_x;
		float gy521_angle_y;
		float gy521_angle_z;
		int8_t gy521_accel_x;
		int8_t gy521_accel_y;
		int8_t gy521_accel_z;
                float core[4];
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
		int data_ready;
	} RoverControlData_t;

	/**
	 * @brief RoverMQTTCommand class is an implementation class extending RoverPahoMQTT for rover-specific topic subscription and publishing using JSON for parsing and using
	 * predefined type variables such as RoverControlData_t and RoverSensorData_t.
	 *
	 */
	class RoverMQTTCommand: private RoverPahoMQTT
	{
		private:
			/**
			 * @brief Given rover identification for multi-agent MQTT communication with a single cloud
			 */
                        int ROVER_ID;

		public:
			/**
			 * @brief Copy constructor for RoverMQTTCommand class that uses username and password
			 */
                        RoverMQTTCommand (char * host, const int port, const int roverID, const int qos, char * username, char * password, char * clientID);

			/**
			 * @brief Copy constructor for RoverMQTTCommand class that doesn't use credentials
			 */
                        RoverMQTTCommand (char * host, const int port, const int roverID, const int qos, char * clientID);

			/**
			 * @brief Destructor for RoverMQTTCommand class
			 */
			~RoverMQTTCommand ();

			/**
			* @brief Publishes a message to rover's telemetry topic (using redirected topic method)
			*
			* Publishing with redirected topics
			*   Rover --->telemetry---> EclipseHono ---> rover/<roverid>/telemetry ---> Rover Telemetry UI
			*
			* @param sensor_data Sensor data to be used in the message
			* @return Return code 0-> success; others-> fail sreturn codes
			*  **1**: Connection refused: Unacceptable protocol version \n
			*	**2**: Connection refused: Identifier rejected \n
			*	**3**: Connection refused: Server unavailable \n
			*	**4**: Connection refused: Bad user name or password \n
			*	**5**: Connection refused: Not authorized \n
			*	**6-255**: Reserved for future use \n
			*	Reference: \cite paho.mqtt.c Asynchronous MQTT Client Documentation
			*/
			int publishToTelemetryTopic (RoverSensorData_t sensor_data);

			/**
			* @brief Publishes a message to rover's telemetry topic (using unredirected topic method)
			*
			* Publishing with non- redirected topics
			*    Rover --> rover/<roverid>/telemetry --> EclipseHono --> rover/<roverid>/telemetry --> Rover Telemetry UI
			*
			* @param sensor_data Sensor data to be used in the message
			* @return Return code 0-> success; others-> fail sreturn codes
			*  **1**: Connection refused: Unacceptable protocol version \n
			*	**2**: Connection refused: Identifier rejected \n
			*	**3**: Connection refused: Server unavailable \n
			*	**4**: Connection refused: Bad user name or password \n
			*	**5**: Connection refused: Not authorized \n
			*	**6-255**: Reserved for future use \n
			*	Reference: \cite paho.mqtt.c Asynchronous MQTT Client Documentation
			*/
			int publishToTelemetryTopicNonRedirected (RoverSensorData_t sensor_data);

			/**
			 * @brief Subscribes to rover's driving topic
			 * @return Return code 0-> success; others-> fail sreturn codes
			 *  **1**: Connection refused: Unacceptable protocol version \n
			 *	**2**: Connection refused: Identifier rejected \n
			 *	**3**: Connection refused: Server unavailable \n
			 *	**4**: Connection refused: Bad user name or password \n
			 *	**5**: Connection refused: Not authorized \n
			 *	**6-255**: Reserved for future use \n
			 *	Reference: \cite paho.mqtt.c Asynchronous MQTT Client Documentation
			 */
			int subscribeToDrivingTopic (void);

			/**
			 * @brief Unsubscribes to rover's driving topic
			 * @return Return code 0-> success; others-> fail sreturn codes
			 *  **1**: Connection refused: Unacceptable protocol version \n
			 *	**2**: Connection refused: Identifier rejected \n
			 *	**3**: Connection refused: Server unavailable \n
			 *	**4**: Connection refused: Bad user name or password \n
			 *	**5**: Connection refused: Not authorized \n
			 *	**6-255**: Reserved for future use \n
			 *	Reference: \cite paho.mqtt.c Asynchronous MQTT Client Documentation
			 */
			int unsubscribeToDrivingTopic (void);

			/**
			 * @brief connectRover connects to the MQTT broker. Each rover ideally should have only one client, instantiated internally within RoverMQTTCommand class.
			 * @return Return code 0-> success, non-zero -> fail
			 *  **1**: Connection refused: Unacceptable protocol version \n
			 *	**2**: Connection refused: Identifier rejected \n
			 *	**3**: Connection refused: Server unavailable \n
			 *	**4**: Connection refused: Bad user name or password \n
			 *	**5**: Connection refused: Not authorized \n
			 *	**6-255**: Reserved for future use \n
			 *	Reference: \cite paho.mqtt.c Asynchronous MQTT Client Documentation
			 */
			int connectRover (void);

			/**
			 * @brief Returns rover connected flag
			 * @return connected flag 1:connected 0: not connected
			 */
			int getRoverConnected(void);

			/**
			 * @brief Returns the retrieved driving information
			 * @return control_data control data that is retrieved
			 */
			RoverControlData_t readFromDrivingTopic (void);

			/**
			 * @brief Retrieves topic name
			 * @return topic_name Topic name
			 */
			char * getTopicName (void);

			/**
			 * @brief Retrieves Rover ID
			 * @return rover_id Rover ID
			 */
			int getRoverID (void);

			/**
			 * @brief Sets Rover ID
			 * @param rover_id Rover ID
			 */
			 void setRoverID (const int rover_id);

	};
}

#endif /* ROVERAPI_ROVER_MQTTCOMMAND_HPP_ */
