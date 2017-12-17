/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    MQTT Publish Task
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created MQTT Publish task 11.12.2017
 *
 */

#include <tasks/mqtt_publish_task.h>

#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_pahomqtt.hpp>
#include <roverapi/rover_mqttcommand.hpp>

#include <wiringPi.h>

void *MQTT_Publish_Task (void * arg)
{
	timing mqtt_publish_task_tmr;

	mqtt_publish_task_tmr.setTaskID("MQTTPublish");
	mqtt_publish_task_tmr.setDeadline(3.0);
	mqtt_publish_task_tmr.setPeriod(3.0);

	RoverMQTTCommand rover_mqtt = RoverMQTTCommand (	"127.0.0.1",//"172.22.167.161",
														1887,//1883,
														1,
														1,
														"rover_mqtt_publisher");
	RoverSensorData_t sensor_data;
	float core_usages[4];

	while (running_flag.get())
	{
		mqtt_publish_task_tmr.recordStartTime();
		mqtt_publish_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		sensor_data.temperature = temperature_shared.get();
		sensor_data.humidity = humidity_shared.get();
		sensor_data.ultrasonic_front = distance_sr04_front_shared.get();
		sensor_data.ultrasonic_rear = distance_sr04_back_shared.get();
		sensor_data.hmc5883l_bearing = bearing_shared.get();
		sensor_data.qmc5883l_bearing = 0.0;
		sensor_data.infrared[0] = infrared_shared[0];
		sensor_data.infrared[1] = infrared_shared[1];
		sensor_data.infrared[2] = infrared_shared[2];
		sensor_data.infrared[3] = infrared_shared[3];
		sensor_data.gy521_bearing = accelerometerdata_shared.bearing;
		sensor_data.gy521_accel_x = accelerometerdata_shared.accel_x;
		sensor_data.gy521_accel_y = accelerometerdata_shared.accel_y;
		sensor_data.gy521_accel_z = accelerometerdata_shared.accel_z;
		sensor_data.gy521_gyro_x = accelerometerdata_shared.gyro_x;
		sensor_data.gy521_gyro_y = accelerometerdata_shared.gyro_y;
		sensor_data.gy521_gyro_z = accelerometerdata_shared.gyro_z;
		sensor_data.gy521_angle_x = accelerometerdata_shared.angle_x;
		sensor_data.gy521_angle_y = accelerometerdata_shared.angle_y;
		sensor_data.gy521_angle_z = accelerometerdata_shared.angle_z;

		if (rover_mqtt.publishToSensorTopic(sensor_data) == 0)
			printf ("Client rover_mqtt_publisher: Publishing successful!\n");
		else
			printf ("Client rover_mqtt_publisher: Publishing unsuccessful!\n");

		core_usages[0] = cpu_util_shared[0];
		core_usages[1] = cpu_util_shared[1];
		core_usages[2] = cpu_util_shared[2];
		core_usages[3] = cpu_util_shared[3];

		if (rover_mqtt.publishToCoreUsageTopic(core_usages) == 0)
			printf ("Client rover_mqtt_publisher: Publishing successful!\n");
		else
			printf ("Client rover_mqtt_publisher: Publishing unsuccessful!\n");




		//Task content ends here -------------------------------------------------

		mqtt_publish_task_tmr.recordEndTime();
		mqtt_publish_task_tmr.calculateExecutionTime();
		mqtt_publish_task_tmr.calculateDeadlineMissPercentage();
		mqtt_publish_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&mqtt_publish_task_ti.mutex);
			mqtt_publish_task_ti.deadline = mqtt_publish_task_tmr.getDeadline();
			mqtt_publish_task_ti.deadline_miss_percentage = mqtt_publish_task_tmr.getDeadlineMissPercentage();
			mqtt_publish_task_ti.execution_time = mqtt_publish_task_tmr.getExecutionTime();
			mqtt_publish_task_ti.period = mqtt_publish_task_tmr.getPeriod();
			mqtt_publish_task_ti.prev_slack_time = mqtt_publish_task_tmr.getPrevSlackTime();
			mqtt_publish_task_ti.task_id = mqtt_publish_task_tmr.getTaskID();
			mqtt_publish_task_ti.start_time = mqtt_publish_task_tmr.getStartTime();
			mqtt_publish_task_ti.end_time = mqtt_publish_task_tmr.getEndTime();
		pthread_mutex_unlock(&mqtt_publish_task_ti.mutex);
		mqtt_publish_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
