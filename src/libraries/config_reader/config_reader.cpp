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
	rover_config configstruct;
	int use_defaults = 0;
	int i = 0;

	FILE *file = fopen (filename, "r");
	printf ("Checking /etc/rover.conf\n");

	if (file != NULL)
	{
		char line[ROVER_CONFIG_MAXBUF];

		while(fgets(line, sizeof(line), file) != NULL)
		{
			char *cfline;
			cfline = strstr((char *)line,DELIM);

			if (cfline != NULL)
			{
				cfline = cfline + strlen(DELIM);

				switch (i)
				{
					case 2:
						if (strlen(cfline) > 1)
						{
							configstruct.ROVER_IDENTITY_C = atoi(cfline);
							printf("Read config [ROVER_IDENTITY_C = %d]\n",configstruct.ROVER_IDENTITY_C);
						}
						else
						{
							configstruct.ROVER_IDENTITY_C = DEFAULT_ROVER_IDENTITY_C;
							printf("No data entered. Using default: [ROVER_IDENTITY_C = %d]\n",configstruct.ROVER_IDENTITY_C);
						}
						break;

					case 3:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.MQTT_BROKER_C,cfline,strlen(cfline));
							configstruct.MQTT_BROKER_C[strlen(cfline)-1] = 0;
							printf("Read config [MQTT_BROKER_C = %s]\n",configstruct.MQTT_BROKER_C);
						}
						else
						{
							memcpy(configstruct.MQTT_BROKER_C,DEFAULT_MQTT_BROKER_C,strlen(DEFAULT_MQTT_BROKER_C));
							configstruct.MQTT_BROKER_C[strlen(DEFAULT_MQTT_BROKER_C)-1] = 0;
							printf("No data entered. Using default: [MQTT_BROKER_C = %s]\n",configstruct.MQTT_BROKER_C);
						}
						break;

					case 4:
						if (strlen(cfline) > 1)
						{
							configstruct.MQTT_BROKER_PORT_C = atoi(cfline);
							printf("Read config [MQTT_BROKER_PORT_C = %d]\n",configstruct.MQTT_BROKER_PORT_C);
						}
						else
						{
							configstruct.MQTT_BROKER_PORT_C = DEFAULT_MQTT_BROKER_PORT_C;
							printf("No data entered. Using default: [MQTT_BROKER_PORT_C = %d]\n",configstruct.MQTT_BROKER_PORT_C);
						}
						break;

					case 5:
						if (strlen(cfline) > 1)
						{
							configstruct.ROVER_MQTT_QOS_C = atoi(cfline);
							printf("Read config [ROVER_MQTT_QOS_C = %d]\n",configstruct.ROVER_MQTT_QOS_C);
						}
						else
						{
							configstruct.ROVER_MQTT_QOS_C = DEFAULT_ROVER_MQTT_QOS_C;
							printf("No data entered. Using default: [ROVER_MQTT_QOS_C = %d]\n",configstruct.ROVER_MQTT_QOS_C);

						}
						break;

					case 6:
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
						break;

					case 7:
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
						break;

					case 8:
						if (strlen(cfline) > 1)
						{
							configstruct.USE_GROOVE_SENSOR_C = atoi(cfline);
							printf("Read config [USE_GROOVE_SENSOR_C = %d]\n",configstruct.USE_GROOVE_SENSOR_C);

						}
						else
						{
							configstruct.USE_GROOVE_SENSOR_C = DEFAULT_USE_GROOVE_SENSOR_C;
							printf("No data entered. Using default: [USE_GROOVE_SENSOR_C = %d]\n",configstruct.USE_GROOVE_SENSOR_C);

						}
						break;

					case 9:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.HONO_HTTP_HOST_C,cfline,strlen(cfline));
							configstruct.HONO_HTTP_HOST_C[strlen(cfline)-1] = 0;
							printf("Read config [HONO_HTTP_HOST_C = %s]\n",configstruct.HONO_HTTP_HOST_C);
						}
						else
						{
							memcpy(configstruct.HONO_HTTP_HOST_C,DEFAULT_HONO_HTTP_HOST_C,strlen(DEFAULT_HONO_HTTP_HOST_C));
							configstruct.HONO_HTTP_HOST_C[strlen(DEFAULT_HONO_HTTP_HOST_C)-1] = 0;
							printf("No data entered. Using default: [HONO_HTTP_HOST_C = %s]\n",configstruct.HONO_HTTP_HOST_C);
						}
						break;

					case 10:
						if (strlen(cfline) > 1)
						{
							configstruct.HONO_HTTP_PORT_C = atoi(cfline);
							printf("Read config [HONO_HTTP_PORT_C = %d]\n",configstruct.HONO_HTTP_PORT_C);
						}
						else
						{
							configstruct.HONO_HTTP_PORT_C = DEFAULT_HONO_HTTP_PORT_C;
							printf("No data entered. Using default: [HONO_HTTP_PORT_C = %d]\n",configstruct.HONO_HTTP_PORT_C);
						}
						break;

					case 11:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.HONO_HTTP_TENANT_NAME_C,cfline,strlen(cfline));
							configstruct.HONO_HTTP_TENANT_NAME_C[strlen(cfline)-1] = 0;
							printf("Read config [HONO_HTTP_TENANT_NAME_C = %s]\n",configstruct.HONO_HTTP_TENANT_NAME_C);
						}
						else
						{
							memcpy(configstruct.HONO_HTTP_TENANT_NAME_C,DEFAULT_HONO_HTTP_TENANT_NAME_C,strlen(DEFAULT_HONO_HTTP_TENANT_NAME_C));
							configstruct.HONO_HTTP_TENANT_NAME_C[strlen(DEFAULT_HONO_HTTP_TENANT_NAME_C)-1] = 0;
							printf("No data entered. Using default: [HONO_HTTP_TENANT_NAME_C = %s]\n",configstruct.HONO_HTTP_TENANT_NAME_C);
						}
						break;

					case 12:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.HONO_HTTP_DEVICE_ID_C,cfline,strlen(cfline));
							configstruct.HONO_HTTP_DEVICE_ID_C[strlen(cfline)-1] = 0;
							printf("Read config [HONO_HTTP_DEVICE_ID_C = %s]\n",configstruct.HONO_HTTP_DEVICE_ID_C);
						}
						else
						{
							memcpy(configstruct.HONO_HTTP_DEVICE_ID_C,DEFAULT_HONO_HTTP_DEVICE_ID_C,strlen(DEFAULT_HONO_HTTP_DEVICE_ID_C));
							configstruct.HONO_HTTP_DEVICE_ID_C[strlen(DEFAULT_HONO_HTTP_DEVICE_ID_C)-1] = 0;
							printf("No data entered. Using default: [HONO_HTTP_DEVICE_ID_C = %s]\n",configstruct.HONO_HTTP_DEVICE_ID_C);
						}
						break;

					case 13:
						if (strlen(cfline) > 1)
						{
							configstruct.HONO_HTTP_REGISTER_PORT_C = atoi(cfline);
							printf("Read config [HONO_HTTP_REGISTER_PORT_C = %d]\n",configstruct.HONO_HTTP_REGISTER_PORT_C);
						}
						else
						{
							configstruct.HONO_HTTP_REGISTER_PORT_C = DEFAULT_HONO_HTTP_REGISTER_PORT_C;
							printf("No data entered. Using default: [HONO_HTTP_REGISTER_PORT_C = %d]\n",configstruct.HONO_HTTP_REGISTER_PORT_C);

						}
						break;

					case 14:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.HONO_HTTP_USERNAME_C,cfline,strlen(cfline));
							configstruct.HONO_HTTP_USERNAME_C[strlen(cfline)-1] = 0;
							printf("Read config [HONO_HTTP_USERNAME_C = %s]\n",configstruct.HONO_HTTP_USERNAME_C);
						}
						else
						{
							memcpy(configstruct.HONO_HTTP_USERNAME_C,DEFAULT_HONO_HTTP_USERNAME_C,strlen(DEFAULT_HONO_HTTP_USERNAME_C));
							configstruct.HONO_HTTP_USERNAME_C[strlen(DEFAULT_HONO_HTTP_USERNAME_C)-1] = 0;
							printf("No data entered. Using default: [HONO_HTTP_USERNAME_C = %s]\n",configstruct.HONO_HTTP_USERNAME_C);
						}
						break;

					case 15:
						if (strlen(cfline) > 1)
						{
							memcpy(configstruct.HONO_HTTP_PASSWORD_C,cfline,strlen(cfline));
							configstruct.HONO_HTTP_PASSWORD_C[strlen(cfline)-1] = 0;
							printf("Read config [HONO_HTTP_PASSWORD_C = %s]\n",configstruct.HONO_HTTP_PASSWORD_C);
						}
						else
						{
							memcpy(configstruct.HONO_HTTP_PASSWORD_C,DEFAULT_HONO_HTTP_PASSWORD_C,strlen(DEFAULT_HONO_HTTP_PASSWORD_C));
							configstruct.HONO_HTTP_PASSWORD_C[strlen(DEFAULT_HONO_HTTP_PASSWORD_C)-1] = 0;
							printf("No data entered. Using default: [HONO_HTTP_PASSWORD_C = %s]\n",configstruct.HONO_HTTP_PASSWORD_C);
						}
						break;

					default:
						break;
				}

			}
			i++;

		} // End while
		fclose(file);
	} // End if file
	else
	{
		use_defaults = 1;
	}

	if (i<15 || use_defaults)
	{
		if (use_defaults)
			printf("Unable to read rover config file. Correct your /etc/rover.conf. Program will use default configuration.\n");
		else
			printf("Corrupted config file. Not all entries are present. Correct your /etc/rover.conf. Program will use default configuration.\n");

		/* Default setup */
		configstruct.ROVER_IDENTITY_C = DEFAULT_ROVER_IDENTITY_C;
		memcpy(configstruct.MQTT_BROKER_C,DEFAULT_MQTT_BROKER_C,strlen(DEFAULT_MQTT_BROKER_C));
		configstruct.MQTT_BROKER_C[strlen(DEFAULT_MQTT_BROKER_C)] = 0;
		configstruct.MQTT_BROKER_PORT_C = DEFAULT_MQTT_BROKER_PORT_C;
		configstruct.ROVER_MQTT_QOS_C = DEFAULT_ROVER_MQTT_QOS_C;
		memcpy(configstruct.MQTT_USERNAME_C,DEFAULT_MQTT_USERNAME_C,strlen(DEFAULT_MQTT_USERNAME_C));
		configstruct.MQTT_USERNAME_C[strlen(DEFAULT_MQTT_USERNAME_C)] = 0;
		memcpy(configstruct.MQTT_PASSWORD_C,DEFAULT_MQTT_PASSWORD_C,strlen(DEFAULT_MQTT_PASSWORD_C));
		configstruct.MQTT_PASSWORD_C[strlen(DEFAULT_MQTT_PASSWORD_C)] = 0;
		configstruct.USE_GROOVE_SENSOR_C = DEFAULT_USE_GROOVE_SENSOR_C;

		memcpy(configstruct.HONO_HTTP_HOST_C,DEFAULT_HONO_HTTP_HOST_C,strlen(DEFAULT_HONO_HTTP_HOST_C));
		configstruct.HONO_HTTP_HOST_C[strlen(DEFAULT_HONO_HTTP_HOST_C)] = 0;
		configstruct.HONO_HTTP_PORT_C = 8080;
		memcpy(configstruct.HONO_HTTP_TENANT_NAME_C,DEFAULT_HONO_HTTP_TENANT_NAME_C,strlen(DEFAULT_HONO_HTTP_TENANT_NAME_C));
		configstruct.HONO_HTTP_TENANT_NAME_C[strlen(DEFAULT_HONO_HTTP_TENANT_NAME_C)] = 0;
		memcpy(configstruct.HONO_HTTP_DEVICE_ID_C,DEFAULT_HONO_HTTP_DEVICE_ID_C,strlen(DEFAULT_HONO_HTTP_DEVICE_ID_C));
		configstruct.HONO_HTTP_DEVICE_ID_C[strlen(DEFAULT_HONO_HTTP_DEVICE_ID_C)] = 0;
		configstruct.HONO_HTTP_REGISTER_PORT_C = DEFAULT_HONO_HTTP_REGISTER_PORT_C;
		memcpy(configstruct.HONO_HTTP_USERNAME_C,DEFAULT_HONO_HTTP_USERNAME_C,strlen(DEFAULT_HONO_HTTP_USERNAME_C));
		configstruct.HONO_HTTP_USERNAME_C[strlen(DEFAULT_HONO_HTTP_USERNAME_C)] = 0;
		memcpy(configstruct.HONO_HTTP_PASSWORD_C,DEFAULT_HONO_HTTP_PASSWORD_C,strlen(DEFAULT_HONO_HTTP_PASSWORD_C));
		configstruct.HONO_HTTP_USERNAME_C[strlen(DEFAULT_HONO_HTTP_PASSWORD_C)] = 0;
		printf ("Using following default configuration:\n");
		printf("[ROVER_IDENTITY_C = %d]\n",configstruct.ROVER_IDENTITY_C);
		printf("[MQTT_BROKER_C = %s]\n",configstruct.MQTT_BROKER_C);
		printf("[MQTT_BROKER_PORT_C = %d]\n",configstruct.MQTT_BROKER_PORT_C);
		printf("[ROVER_MQTT_QOS_C = %d]\n",configstruct.ROVER_MQTT_QOS_C);
		printf("[MQTT_USERNAME_C = %s]\n",configstruct.MQTT_USERNAME_C);
		printf("[MQTT_PASSWORD_C = %s]\n",configstruct.MQTT_PASSWORD_C);
		printf("[USE_GROOVE_SENSOR_C = %d]\n",configstruct.USE_GROOVE_SENSOR_C);

		printf("[HONO_HTTP_HOST_C = %s]\n",configstruct.HONO_HTTP_HOST_C);
		printf("[HONO_HTTP_PORT_C = %d]\n",configstruct.HONO_HTTP_PORT_C);
		printf("[HONO_HTTP_TENANT_NAME_C = %s]\n",configstruct.HONO_HTTP_TENANT_NAME_C);
		printf("[HONO_HTTP_DEVICE_ID_C = %s]\n",configstruct.HONO_HTTP_DEVICE_ID_C);
		printf("[HONO_HTTP_REGISTER_PORT_C = %d]\n",configstruct.HONO_HTTP_REGISTER_PORT_C);
		printf("[HONO_HTTP_USERNAME_C = %s]\n",configstruct.HONO_HTTP_USERNAME_C);
		printf("[HONO_HTTP_PASSWORD_C = %s]\n",configstruct.HONO_HTTP_PASSWORD_C);
	}

	return configstruct;
}

