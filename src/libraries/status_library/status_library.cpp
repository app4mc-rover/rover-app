/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Status library for getting WiFi, Ethernet Interface, Internet, Bluetooth, Hono Cloud status
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 04.10.2017
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libraries/status_library/status_library.h>
#include <libraries/hono_interaction/hono_interaction.h>

/* Returns 1 for ON, 0 for OFF
   Assuming the connection is named by default as 'wlan0' */
int retrieveWLANStatus (void)
{
	FILE *fp = nullptr;
	char buffer[2] = {};

	/* Execute the command */
	fp = popen("ifconfig wlan0 2>&1 | grep 'RUNNING' | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

	pclose(fp);

	/* Return */
	if (buffer[0] == '0')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* Returns 1 for ON, 0 for OFF
   Assuming the connection is named by default as 'eth0' */
int retrieveETHStatus (void)
{
	FILE *fp = nullptr;
	char buffer[2] = {};

	/* Execute the command */
	fp = popen("ifconfig eth0 2>&1 | grep 'RUNNING' | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

	pclose(fp);

	/* Return */
	if (buffer[0] == '0')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* Returns 1 for ON, 0 for OFF */
int retrieveINTERNETStatus (void)
{
	FILE *fp = nullptr;
	char buffer[2] = {};

	/* Execute the command */
	fp = popen("ping -q -w 1 -c 1 `ip r | grep default | cut -d ' ' -f 3` 1>/dev/null 2>/dev/null && echo 1 || echo 0","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

	pclose(fp);

	/* Return */
	if (buffer[0] == '0')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// Returns 1 for ON, 0 for OFF
int retrieveBLUETOOTHStatus (void)
{
	FILE *fp = nullptr;
	char buffer[2] = {};

	// Execute the command
	fp = popen("systemctl status bluetooth | grep inactive | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

	pclose(fp);

	/* Return */
	if (buffer[0] == '1')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// Returns 1 for ON, 0 for OFF
// hono_interaction.cpp -> registerEntriesToHonoInstance() should be called first. It is called in main()
int retrieveHONOStatus (void)
{
	/* Dumb way to do it :) TODO: Make this more generic */
	FILE *fp = nullptr;
	char buffer[20] = {};
	int code = 0;
	int status = 0;

	/* Execute the command */
	fp = popen("curl -X POST -i -u sensor1@DEFAULT_TENANT:hono-secret -H 'Content-Type: application/json' --data-binary '{\"connection_check\": 1}' http://idial.institute:8080/telemetry 2>/dev/null","r");

	/* Read from pipe */
	fgets(buffer, 13, fp); //Get the string HTTP/1.1 XXX

	/* Prepare the response code XXX */
	sscanf(buffer, "HTTP/1.1 %d", &code);

	/* Debug */
	//printf("Response=%s\n",buffer);

	/* Get status */
	status = handleCode(code);

	pclose(fp);

	/* Return */
	if (status == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int retrieveHONOStatus (char * host_name, int port, char * tenant_name, char * device_id, char * user, char * password)
{
	FILE *fp = nullptr;
	int code = 0;
	int status = 0;

	char buffer[256] = {};
	char num_buffer[33] = {};

	//Prepare command as string
	//Example: "curl -X PUT -i -H 'Content-Type: application/json' --data-binary '{"Bearing": 0.5}' http://idial.institute:8080/telemetry/DEFAULT_TENANT/4711"

	//For Hono 0.5-M9
	//Example: "curl -X POST -i -u sensor1@DEFAULT_TENANT:hono-secret -H 'Content-Type: application/json' --data-binary '{"temp": 5}' http://idial.institute:8080/telemetry"

	//To get the information in dashboard, we use device ID as the entry name, and "value" as field.
	//Example: "curl -X PUT -i -H 'Content-Type: application/json' --data-binary '{"value": 0.5}' http://idial.institute:8080/telemetry/DEFAULT_TENANT/roverRearSensor"

	sprintf(buffer, "curl -X POST -i -u ");
	strcat(buffer, user);
	strcat(buffer, "@");
	strcat(buffer, tenant_name);
	strcat(buffer, ":");
	strcat(buffer, password);
	strcat(buffer, " -H 'Content-Type: application/json' --data-binary '{\"");

	strcat(buffer, "datacheck");
	strcat(buffer, "\": ");
	snprintf(num_buffer, sizeof(num_buffer), "%f", 1.0);
	strcat(buffer, num_buffer);
	num_buffer[0] = 0; //Clear array

	strcat(buffer, "}' http://");
	strcat(buffer, host_name);
	strcat(buffer, ":");
	snprintf(num_buffer, sizeof(num_buffer), "%d", port);
	strcat(buffer, num_buffer);
	num_buffer[0] = 0; //Clear array
	strcat(buffer, "/telemetry");

	//To redirect pipe to prevent stdout showing all outputs generated by curl
	strcat(buffer, " 2>/dev/null"); //2>&1 would redirect to stderr, we choose to be able to parse returned code

	//Execute the command
	fp = popen(buffer,"r");

	//Get and Parse the output
	fgets(buffer, 13, fp); //Get the string HTTP/1.1 XXX

	//Prepare the response code
	sscanf(buffer, "HTTP/1.1 %d", &code);

	/* Debug */
	//printf("Response=%s\n",buffer);

	/* Get status */
	status = handleCode(code);

	pclose(fp);

	/* Return */
	if (status == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


