/*
 * Copyright (c) 2018 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Configuration file reader for the rover
 *    	Header file
 *    Configuration file is read from /etc/rover.conf. Sample is located at samples/rover.conf.sample
 *
 * Author:
 *    M. Ozcelikors,  <mozcelikors@gmail.com> - created 18.01.2018
 */

#ifndef LIBRARIES_CONFIG_READER_CONFIG_READER_H_
#define LIBRARIES_CONFIG_READER_CONFIG_READER_H_


#define ROVER_CONFIG_MAXBUF 1024
#define DELIM "="
#define ROVER_CONFIG_FILE "/etc/rover.conf"

#define DEFAULT_ROVER_IDENTITY_C            (int) 1
#define DEFAULT_MQTT_BROKER_C               (char*) "127.0.0.1"
#define DEFAULT_MQTT_BROKER_PORT_C          (int) 1883
#define DEFAULT_ROVER_MQTT_QOS_C            (int) 0
#define DEFAULT_MQTT_USERNAME_C             (char*) "sensor1@DEFAULT_TENANT"
#define DEFAULT_MQTT_PASSWORD_C             (char*) "hono-secret"
#define DEFAULT_USE_GROOVE_SENSOR_C         (int) 0
#define DEFAULT_HONO_HTTP_HOST_C            (char*) "idial.institute"
#define DEFAULT_HONO_HTTP_PORT_C            (int) 8080
#define DEFAULT_HONO_HTTP_TENANT_NAME_C     (char*) "DEFAULT_TENANT"
#define DEFAULT_HONO_HTTP_DEVICE_ID_C       (char*) "4711"
#define DEFAULT_HONO_HTTP_REGISTER_PORT_C   (int) 28080
#define DEFAULT_HONO_HTTP_USERNAME_C        (char*) "sensor1"
#define DEFAULT_HONO_HTTP_PASSWORD_C        (char*) "hono-secret"

typedef struct
{
        int ROVER_IDENTITY_C;
	char MQTT_BROKER_C [ROVER_CONFIG_MAXBUF];
	int MQTT_BROKER_PORT_C;
	int ROVER_MQTT_QOS_C;
	char MQTT_USERNAME_C [ROVER_CONFIG_MAXBUF];
	char MQTT_PASSWORD_C [ROVER_CONFIG_MAXBUF];
	int USE_GROOVE_SENSOR_C;
        char HONO_HTTP_HOST_C [ROVER_CONFIG_MAXBUF];
        int HONO_HTTP_PORT_C;
        char HONO_HTTP_TENANT_NAME_C [ROVER_CONFIG_MAXBUF];
        char HONO_HTTP_DEVICE_ID_C [ROVER_CONFIG_MAXBUF];
        int HONO_HTTP_REGISTER_PORT_C;
        char HONO_HTTP_USERNAME_C [ROVER_CONFIG_MAXBUF];
        char HONO_HTTP_PASSWORD_C [ROVER_CONFIG_MAXBUF];
} rover_config;

rover_config getRoverConfig (char *filename);

#endif /* LIBRARIES_CONFIG_READER_CONFIG_READER_H_ */
