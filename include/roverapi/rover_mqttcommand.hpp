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
// Topic naming is such as the following:
//  rover/<roverID 1-99>/RoverDriving/control
//  e.g rover/1/RoverDriving/Control
#define topicPrefix "rover/"
#define drivingSubTopic "/RoverDriving/control"
#define sensorSubTopic  "/RoverSensor/sensors"
#define coreSubTopic "/RoverCore/usage"

/* Buffer size for received message for MQTT */
#define MQTT_BUFSIZE 256

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
		int data_ready;
	} RoverControlData_t;

	/**
	 * @brief RoverMQTTCommand class is an implementation class extending RoverPahoMQTT for rover-specific topic subscription and publishing using JSON for parsing and using
	 * predefined type variables such as RoverControlData_t and RoverSensorData_t.
	 *
	 * Example usage of this class as a <b>publisher client</b> is given below:
	 *
	 * \code{.cpp}
	 *
	 *		#include <roverapi/rover_mqttcommand.hpp>
	 *
	 *		RoverMQTTCommand rover_mqtt = RoverMQTTCommand (	"127.0.0.1",
	 *															1883,
	 *															1,
	 *															"rover_mqtt_publisher");
	 *		RoverSensorData_t sensor_data;
	 *		sensor_data.temperature = temperature_shared.get();
	 *		sensor_data.humidity = humidity_shared.get();
	 *		sensor_data.ultrasonic_front = distance_sr04_front_shared.get();
	 *		sensor_data.ultrasonic_rear = distance_sr04_back_shared.get();
	 *		sensor_data.hmc5883l_bearing = bearing_shared.get();
	 *		sensor_data.qmc5883l_bearing = 0.0;
	 *		sensor_data.infrared[0] = infrared_shared[0];
	 *		sensor_data.infrared[1] = infrared_shared[1];
	 *		sensor_data.infrared[2] = infrared_shared[2];
	 *		sensor_data.infrared[3] = infrared_shared[3];
	 *		sensor_data.gy521_bearing = accelerometerdata_shared.bearing;
	 *		sensor_data.gy521_accel_x = accelerometerdata_shared.accel_x;
	 *		sensor_data.gy521_accel_y = accelerometerdata_shared.accel_y;
	 *		sensor_data.gy521_accel_z = accelerometerdata_shared.accel_z;
	 *		sensor_data.gy521_gyro_x = accelerometerdata_shared.gyro_x;
	 *		sensor_data.gy521_gyro_y = accelerometerdata_shared.gyro_y;
	 *		sensor_data.gy521_gyro_z = accelerometerdata_shared.gyro_z;
	 *		sensor_data.gy521_angle_x = accelerometerdata_shared.angle_x;
	 *		sensor_data.gy521_angle_y = accelerometerdata_shared.angle_y;
	 *		sensor_data.gy521_angle_z = accelerometerdata_shared.angle_z;
	 *
	 *		if (rover_mqtt.publishToSensorTopic(sensor_data) == 0)
	 *			printf ("Client rover_mqtt_publisher: Publishing successful!\n");
	 *		else
	 *			printf ("Client rover_mqtt_publisher: Publishing unsuccessful!\n");
	 *
	 * \endcode
	 *
	 * Example usage of this class as a <b>subscriber client</b> is given below:
	 *
	 * \code{.cpp}
	 *
	 *  #include <roverapi/rover_mqttcommand.hpp>
	 *
	 * 	RoverMQTTCommand rover_mqtt = RoverMQTTCommand ("127.0.0.1",
	 *													1883,
	 *													1,
	 *													"rover_mqtt_subscriber");
	 * 	RoverControlData_t control_data;
	 *
	 * 	if (rover_mqtt.subscribeToDrivingTopic() == 0)
	 * 	{
	 * 		printf ("Client rover_mqtt_subscriber: Subscription succesful!\n");
	 * 	}
	 * 	else
	 * 	{
	 * 		printf ("Client rover_mqtt_subscriber: Subscription unsuccessful!\n");
	 * 	}
	 *
	 * 	control_data = rover_mqtt.readFromDrivingTopic();
	 *
	 * 	//Override driving-related global variables if data is ready
	 * 	if (control_data.data_ready == 1)
	 * 	{
	 * 		speed_shared = control_data.speed;
	 * 		keycommand_shared = control_data.command;
	 * 		driving_mode = control_data.driving_mode;
	 * 		printf ("Client rover_mqtt_subscriber: Data received: command=%c speed=%d mode=%d\n",
	 *				                                              control_data.command,
	 *															  control_data.speed,
	 *															  control_data.driving_mode);
	 *	}
	 *
	 *	\endcode
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
			 * @brief Copy constructor for RoverMQTTCommand class
			 */
			RoverMQTTCommand (char * host, const int port, const int roverID, const int qos, char * clientID);

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
			 * @brief Publishes a mesage to rover's core usage topic
			 * @param core_usages Core usage array with 4 cores
			 * @return int Return code
			 */
			int publishToCoreUsageTopic (float* core_usages);

			/**
			 * @brief Subscribes to rover's driving topic
			 */
			int subscribeToDrivingTopic (void);

			/**
			 * @brief Unsubscribes to rover's driving topic
			 */
			int unsubscribeToDrivingTopic (void);

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
