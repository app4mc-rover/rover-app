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

#include <api/rover_cloud.hpp>
#include <libraries/hono_interaction/hono_interaction.h>
#include <libraries/status_library/status_library.h>


int rover::RoverCloud::registerDevice (char * device_id)
{
	int status = 0;
	status = registerDeviceToHonoInstance (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id);
	return status;
}

int rover::RoverCloud::sendTelemetry (char * device_id, char * user, char * password, char * field, double value)
{
	int status = 0;
	status = sendTelemetryDataToHonoInstance (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password, field, value);
	return status;
}

int rover::RoverCloud::sendEvent (char * device_id, char * user, char * password, char * field, double value)
{
	int status = 0;
	status = sendEventDataToHonoInstance (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password, field, value);
	return status;
}

void rover::RoverCloud::setHono (char * host_name, int port, char * tenant)
{
	this->HOST_NAME = host_name;
	this->PORT = port;
	this->TENANT_NAME = tenant;
}

void rover::RoverCloud::setHostName (char * host_name)
{
	this->HOST_NAME = host_name;
}

void rover::RoverCloud::setPort (int port)
{
	this->PORT = port;
}

void rover::RoverCloud::setTenantName (char * tenant)
{
	this->TENANT_NAME = tenant;
}

char * rover::RoverCloud::getHostName (void)
{
	return this->HOST_NAME;
}

int  rover::RoverCloud::getPort (void)
{
	return this->PORT;
}

char * rover::RoverCloud::getTenantName (void)
{
	return this->TENANT_NAME;
}

int rover::RoverCloud::getHonoStatus (char * device_id, char * user, char * password)
{
	int status = 0;
	status = retrieveHONOStatus (this->HOST_NAME, this->PORT, this->TENANT_NAME, device_id, user, password);
	return status;
}
