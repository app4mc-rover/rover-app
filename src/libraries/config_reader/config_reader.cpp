/*
 * Copyright (c) 2018 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Configuration file reader for the rover
 *    Configuration file is read from /etc/rover.conf. Sample is located at samples/rover.conf.sample
 *
 * Author:
 *    M. Ozcelikors,  <mozcelikors@gmail.com> - created 18.01.2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libraries/config_reader/config_reader.h>

rover_config getRoverConfig(char *filename)
{
	rover_config configstruct = {1, "127.0.0.1", 1883, 0, "", "", 0};
	FILE *file = fopen (filename, "r");
	printf ("Checking /etc/rover.conf\n");

	if (file != NULL)
	{
		char line[ROVER_CONFIG_MAXBUF];
		int i = 0;

		while(fgets(line, sizeof(line), file) != NULL)
		{
			char *cfline;
			cfline = strstr((char *)line,DELIM);

			if (cfline != NULL)
			{
				cfline = cfline + strlen(DELIM);

				if (i == 2)
				{
					configstruct.ROVER_IDENTITY_C = atoi(cfline);
					printf("Read config [ROVER_IDENTITY_C = %d]\n",configstruct.ROVER_IDENTITY_C);
				}
				else if (i == 3)
				{
					memcpy(configstruct.MQTT_BROKER_C,cfline,strlen(cfline));
					configstruct.MQTT_BROKER_C[strlen(cfline)-1] = 0;
					printf("Read config [MQTT_BROKER_C = %s]\n",configstruct.MQTT_BROKER_C);
				}
				else if (i == 4)
				{
					configstruct.MQTT_BROKER_PORT_C = atoi(cfline);
					printf("Read config [MQTT_BROKER_PORT_C = %d]\n",configstruct.MQTT_BROKER_PORT_C);
				}
				else if (i == 5)
				{
					configstruct.ROVER_MQTT_QOS_C = atoi(cfline);
					printf("Read config [ROVER_MQTT_QOS_C = %d]\n",configstruct.ROVER_MQTT_QOS_C);
				}
				else if (i == 6)
				{
					if (strlen(cfline) > 1)
					{
						memcpy(configstruct.MQTT_USERNAME_C,cfline,strlen(cfline));
						configstruct.MQTT_USERNAME_C[strlen(cfline)-1] = 0;
						printf("Read config [MQTT_USERNAME_C = %s]\n",configstruct.MQTT_USERNAME_C);
					}
					else
					{
						printf("Read config [MQTT_USERNAME_C = <no-username>]\n");

					}
				}
				else if (i == 7)
				{
					if (strlen(cfline) > 1)
					{
						memcpy(configstruct.MQTT_PASSWORD_C,cfline,strlen(cfline));
						configstruct.MQTT_PASSWORD_C[strlen(cfline)-1] = 0;
						printf("Read config [MQTT_PASSWORD_C = %s]\n",configstruct.MQTT_PASSWORD_C);
					}
					else
					{
						printf("Read config [MQTT_PASSWORD_C = <no-password>]\n");

					}
				}
				else if (i == 8)
				{
					configstruct.USE_GROOVE_SENSOR_C = atoi(cfline);
					printf("Read config [USE_GROOVE_SENSOR_C = %d]\n",configstruct.USE_GROOVE_SENSOR_C);
				}

			}
			i++;

		} // End while
		fclose(file);
	} // End if file
	else
	{
        printf("Unable to read rover config file. Correct your /etc/rover.conf. Program will use default configuration.\n");

		/* Default setup */
		configstruct.ROVER_IDENTITY_C = 1;
        memcpy(configstruct.MQTT_BROKER_C,"127.0.0.1",strlen("127.0.0.1"));
        configstruct.MQTT_BROKER_PORT_C = 1883;
		configstruct.ROVER_MQTT_QOS_C = 0;
        memcpy(configstruct.MQTT_USERNAME_C,"sensor1@DEFAULT_TENANT",strlen("sensor1@DEFAULT_TENANT"));
        memcpy(configstruct.MQTT_PASSWORD_C,"hono-secret",strlen("hono-secret"));
        configstruct.USE_GROOVE_SENSOR_C = 0;

		printf ("Using following default configuration:\n");
		printf("[ROVER_IDENTITY_C = %d]\n",configstruct.ROVER_IDENTITY_C);
		printf("[MQTT_BROKER_C = %s]\n",configstruct.MQTT_BROKER_C);
		printf("[MQTT_BROKER_PORT_C = %d]\n",configstruct.MQTT_BROKER_PORT_C);
		printf("[ROVER_MQTT_QOS_C = %d]\n",configstruct.ROVER_MQTT_QOS_C);
		printf("[MQTT_USERNAME_C = %s]\n",configstruct.MQTT_USERNAME_C);
		printf("[MQTT_PASSWORD_C = %s]\n",configstruct.MQTT_PASSWORD_C);
		printf("[USE_GROOVE_SENSOR_C = %d]\n",configstruct.USE_GROOVE_SENSOR_C);
	}

	return configstruct;
}

