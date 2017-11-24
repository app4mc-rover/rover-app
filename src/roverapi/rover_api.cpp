/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover API - Interfaces for Rover application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_api.hpp>
#include <wiringPi.h>

/**
  *   @brief  Constructor for the RoverBase class
  */
rover::RoverBase::RoverBase()
{

}

/**
  *   @brief  Destructor for the RoverBase class
  */
rover::RoverBase::~RoverBase()
{

}

/**
  *   @brief  Initializes the libraries for the Rover
  */
void rover::RoverBase::initialize(void)
{
	/* wiringPi can only be called once */
#ifndef _WIRINGPI_SETUP
#define _WIRINGPI_SETUP
	wiringPiSetup ();
#endif
}


