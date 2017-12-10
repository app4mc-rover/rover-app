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

#define RoverDrivingTopic 					"rover/RoverDriving/control"
#define RoverSensorTopic_All 				"rover/RoverSensor/sensors"

#define RoverSensorTopic_dht22_temperatyre	"rover/RoverSensor/dht22/temperature"
#define RoverSensorTopic_dht22_humidity		"rover/RoverSensor/dht22/humidity"
#define RoverSensorTopic_infrared0			"rover/RoverSensor/infrared/0"
#define RoverSensorTopic_infrared1			"rover/RoverSensor/infrared/1"
#define RoverSensorTopic_infrared2			"rover/RoverSensor/infrared/2"
#define RoverSensorTopic_infrared3			"rover/RoverSensor/infrared/3"
#define RoverSensorTopic_ultrasonic_front	"rover/RoverSensor/ultrasonic/front"
#define RoverSensorTopic_ultrasonic_rear	"rover/RoverSensor/ultrasonic/rear"
#define RoverSensorTopic_hmc5883l_bearing	"rover/RoverSensor/hmc5883l/bearing"
#define RoverSensorTopic_qmc5883l_bearing	"rover/RoverSensor/qmc5883l/bearing"
#define RoverSensorTopic_gy521_bearing		"rover/RoverSensor/gy521/gyro/bearing"
#define RoverSensorTopic_gy521_gyro_x		"rover/RoverSensor/gy521/gyro/x"
#define RoverSensorTopic_gy521_gyro_y		"rover/RoverSensor/gy521/gyro/y"
#define RoverSensorTopic_gy521_gyro_z		"rover/RoverSensor/gy521/gyro/z"
#define RoverSensorTopic_gy521_accel_x		"rover/RoverSensor/gy521/accel/x"
#define RoverSensorTopic_gy521_accel_y		"rover/RoverSensor/gy521/accel/y"
#define RoverSensorTopic_gy521_accel_z		"rover/RoverSensor/gy521/accel/z"
#define RoverSensorTopic_gy521_angle_x		"rover/RoverSensor/gy521/angle/x"
#define RoverSensorTopic_gy521_angle_y		"rover/RoverSensor/gy521/angle/y"
#define RoverSensorTopic_gy521_angle_z		"rover/RoverSensor/gy521/angle/z"

namespace rover
{
	class RoverMQTTCommand: private RoverPahoMQTT
	{
		private:
			char* HOST_NAME;
			int PORT;

		public:
			RoverMQTTCommand (char * host, const int port, const int qos, char * clientID);
			~RoverMQTTCommand ();

	};
}



#endif /* ROVERAPI_ROVER_MQTTCOMMAND_HPP_ */
