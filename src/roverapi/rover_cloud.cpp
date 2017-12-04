/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Cloud API - Interfaces for Rover cloud application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_cloud.hpp>

#include <libraries/hono_interaction/hono_interaction.h>
#include <libraries/status_library/status_library.h>

#include <stdio.h>
#include <string.h>

rover::RoverCloud::RoverCloud()
:REGISTRATION_PORT(1),
 TENANT_NAME("N"),
 PORT(1),
 HOST_NAME("N")
{

}

rover::RoverCloud::RoverCloud(char * host_name, const int port, const int registration_port, char * tenant_name)
{
	this->HOST_NAME = host_name;
	this->PORT = port;
	this->REGISTRATION_PORT = registration_port;
	this->TENANT_NAME = tenant_name;
}

int rover::RoverCloud::attributeErrorCheck (void) const
{
	if (this->PORT == 1 || this->TENANT_NAME[0] == 'N' || this->HOST_NAME[0] == 'N')
	{
		fprintf (stderr, "Hono is not initialized. Please use: setHono function\n");
		return 0;
	}
	else
	{
		return 1;
	}
}

int rover::RoverCloud::registerDevice (char * device_id)
{
	if (this->REGISTRATION_PORT != 1)
	{
		int status = 0;
		status = registerDeviceToHonoInstance (this->HOST_NAME, this->REGISTRATION_PORT, this->TENANT_NAME, device_id);
		return status;
	}
	else
	{
		fprintf (stderr, "Registration port is not initialized. Please use: setRegistrationPort function\n");
		return 0;
	}
}

int rover::RoverCloud::sendTelemetry (char * device_id, char * user, char * password, char * field, double value)
{
	if (this->attributeErrorCheck() == 1)
	{
		int status = 0;
		status = sendTelemetryDataToHonoInstance (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password, field, value);
		return status;
	}
	else
	{
		return 0;
	}
}

int rover::RoverCloud::sendEvent (char * device_id, char * user, char * password, char * field, double value)
{
	if (this->attributeErrorCheck() == 1)
	{
		int status = 0;
		status = sendEventDataToHonoInstance (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password, field, value);
		return status;
	}
	else
	{
		return 0;
	}
}

void rover::RoverCloud::setHono (char * host_name, const int port, char * tenant) const
{

	this->HOST_NAME = host_name;
	this->PORT = port;
	this->TENANT_NAME = tenant;
}

void rover::RoverCloud::setHostName (char * host_name) const
{
	this->HOST_NAME = host_name;
}

void rover::RoverCloud::setPort (const int port) const
{
	this->PORT = port;
}

void rover::RoverCloud::setRegistrationPort (const int port) const
{
	this->REGISTRATION_PORT = port;
}

int rover::RoverCloud::getRegistrationPort (void) const
{
	return this->REGISTRATION_PORT;
}

void rover::RoverCloud::setTenantName (char * tenant) const
{
	this->TENANT_NAME = tenant;
}

char * rover::RoverCloud::getHostName (void) const
{
	return this->HOST_NAME;
}

int  rover::RoverCloud::getPort (void) const
{
	return this->PORT;
}

char * rover::RoverCloud::getTenantName (void) const
{
	return this->TENANT_NAME;
}

int rover::RoverCloud::getHonoStatus (char * device_id, char * user, char * password)
{
	if (this->attributeErrorCheck() == 1)
	{
		int status = 0;
		status = retrieveHONOStatus (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password);
		return status;
	}
	else
	{
		return 0;
	}
}
