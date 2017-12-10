/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Utils API - Interfaces for Rover utilities application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_utils.hpp>

#include <libraries/status_library/status_library.h>

void rover::RoverUtils::getCoreUtilization (float coreUtil[numberOfCores])
{
#if SIMULATOR
	return;
#endif
	FILE *fp;
	char buffer[128];
	float util[5];
	size_t bytes_read;

	/* Execute the command */
	fp = popen("python /opt/rover-app/scripts/read_core_usage.py ","r");

	/* Read to buffer */
	bytes_read = fread(buffer, 1, sizeof(buffer), fp);

	if (bytes_read == 0 || bytes_read == sizeof(buffer))
		perror("Can't read from /opt/rover-app/scripts/read_core_usage.py");

	buffer[bytes_read] = '\0';

	//printf("buf:%s\n",buffer);

	/* Parse */
	sscanf(buffer,"[%f, %f, %f, %f]",&coreUtil[0], &coreUtil[1], &coreUtil[2], &coreUtil[3]);

}

int rover::RoverUtils::getWlanStatus (void)
{
	return retrieveWLANStatus ();
}

int rover::RoverUtils::getEthernetStatus (void)
{
	return retrieveETHStatus ();
}

int rover::RoverUtils::getInternetStatus (void)
{
	return retrieveINTERNETStatus ();
}

int rover::RoverUtils::getBluetoothStatus (void)
{
	return retrieveBLUETOOTHStatus ();
}

int rover::RoverUtils::getHonoCloudStatus (char * host_name, int port, char * tenant_name, char * device_id, char * user, char * password)
{
	return retrieveHONOStatus (host_name, port, tenant_name, device_id, user, password);
}
