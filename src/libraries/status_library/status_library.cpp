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

#include <libraries/status_library/status_library.h>
#include <libraries/hono_interaction/hono_interaction.h>

/* Returns 1 for ON, 0 for OFF
   Assuming the connection is named by default as 'wlan0' */
int retrieveWLANStatus (void)
{
	FILE *fp;
	char buffer[2];

	/* Execute the command */
	fp = popen("ifconfig wlan0 2>&1 | grep 'RUNNING' | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

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
	FILE *fp;
	char buffer[2];

	/* Execute the command */
	fp = popen("ifconfig eth0 2>&1 | grep 'RUNNING' | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

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
	FILE *fp;
	char buffer[2];

	/* Execute the command */
	fp = popen("ping -q -w 1 -c 1 `ip r | grep default | cut -d ' ' -f 3` 1>/dev/null 2>/dev/null && echo 1 || echo 0","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

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
	FILE *fp;
	char buffer[2];

	// Execute the command
	fp = popen("service bluetooth status | grep inactive | wc -l","r");

	/* Read from pipe */
	fgets(buffer, 2, fp);

	/* Test the output char */
	//printf("buf=%c\n",buffer[0]);

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
	FILE *fp;
	char buffer[20];
	int code;
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


