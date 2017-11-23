/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Sensors API - Interfaces for Rover sensors application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_sensors.hpp>

#include <roverapi/basic_psys_rover.h>
#include <mcp3004.h>

void rover::RoverSensors::initialize (void)
{
	/* wiringPi can only be called once */
#ifndef _WIRINGPI_SETUP
#define _WIRINGPI_SETUP
	wiringPiSetup ();
#endif

	/* Init the analog digital converter */
	mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels
}
