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

#include <roverapi/rover_cloud.hpp>
#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_gpio.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_sensors.hpp>

/**
  *   @brief  Constructor for the RoverBase class
  */
rover::RoverBase::RoverBase():
WIRINGPI_INIT_(0),
ROVER_DISPLAY_INIT_(0),
ROVER_DRIVING_INIT_(0),
ROVER_GPIO_INIT_(0),
ROVER_SENSORS_INIT_(0)
{

}

/**
  *   @brief  Destructor for the RoverBase class
  */
rover::RoverBase::~RoverBase()
{

}


void rover::RoverBase::initialize(void)
{
	this->initializeWiringPi();
	this->initializeRoverDisplay();
	this->initializeRoverDriving();
	this->initializeRoverGpio();
	this->initializeRoverSensors();
}


void rover::RoverBase::initializeWiringPi(void)
{
	/* wiringPi can only be called once per program, One solution: */
	static class Once { public: Once(){
		wiringPiSetup();
		printf("wiringPi Setup Done..\n");
	}} Once_;
	this->WIRINGPI_INIT_ = 1;
}

void rover::RoverBase::shutdown (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DISPLAY_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDisplay. Use RoverBase::initialize() !\n");
	}
	else
	{
		RoverDisplay my_display = this->inRoverDisplay();

		/* Prepare "Shutting Down..." */
		my_display.clearDisplay();

		my_display.setTextSize(2);
		my_display.setTextColor(WHITE);

		my_display.setCursor(20,10);
		my_display.print("Shutting");

		my_display.setTextColor(WHITE);

		my_display.setCursor(20,32);
		my_display.print("Down...");

		/* Display everything earlier this time*/
		my_display.display();

		/* Play the shutdown tone..*/
		this->inRoverGpio().shutdownTone();

		/* Prepare "Shutting Down..." */
		my_display.clearDisplay();

		my_display.setTextSize(2);
		my_display.setTextColor(WHITE);

		my_display.setCursor(20,10);
		my_display.print("Shutting");

		my_display.setTextColor(WHITE);

		my_display.setCursor(20,32);
		my_display.print("Down...");

		/* Display everything earlier this time*/
		my_display.display();

		/* Here we're shutting Raspberry Pi down.. */
		system("halt");

		/* Prepare "Shutting Down..." */
		my_display.clearDisplay();

		my_display.setTextSize(2);
		my_display.setTextColor(WHITE);

		my_display.setCursor(20,10);
		my_display.print("Shutting");

		my_display.setTextColor(WHITE);

		my_display.setCursor(20,32);
		my_display.print("Down...");

		/* Display everything earlier this time*/
		my_display.display();

		/* Abort the application for safety reasons */
		abort();
	}
}

void rover::RoverBase::sleep (const unsigned int period_ms)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverBase::initialize() !\n");
	}
	else
	{
		delay (period_ms);
	}
}

rover::RoverUtils& rover::RoverBase::inRoverUtils (void)
{
	return this->myRoverUtils;
}

rover::RoverCloud& rover::RoverBase::inRoverCloud (void)
{
	return this->myRoverCloud;
}

rover::RoverDriving& rover::RoverBase::inRoverDriving (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverBase::initialize() !\n");
	}
	else
	{
		return this->myRoverDriving;
	}
}

rover::RoverGpio& rover::RoverBase::inRoverGpio (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_GPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDisplay. Use RoverGpio::initialize() !\n");
	}
	else
	{
		return this->myRoverGpio;
	}
}

rover::RoverDisplay& rover::RoverBase::inRoverDisplay (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DISPLAY_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDisplay. Use RoverBase::initialize() !\n");
	}
	else
	{
		return this->myRoverDisplay;
	}
}

rover::RoverSensors& rover::RoverBase::inRoverSensors (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_SENSORS_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverSensors. Use RoverBase::initialize() !\n");
	}
	else
	{
		return this->myRoverSensors;
	}
}

void rover::RoverBase::initializeRoverSensors (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_SENSORS_INIT_ != 1)
	{
		this->myRoverSensors.initialize();
		this->ROVER_SENSORS_INIT_ = 1;
	}
}

void rover::RoverBase::initializeRoverGpio (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_GPIO_INIT_ != 1)
	{
		this->myRoverGpio.initialize();
		this->ROVER_GPIO_INIT_ = 1;
	}
}

void rover::RoverBase::initializeRoverDisplay (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DISPLAY_INIT_ != 1)
	{
		this->myRoverDisplay.initialize();
		this->ROVER_DISPLAY_INIT_ = 1;
	}
}

void rover::RoverBase::initializeRoverDriving (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else if (this->ROVER_DRIVING_INIT_ != 1)
	{
		this->myRoverDriving.initialize();
		this->ROVER_DRIVING_INIT_ = 1;
	}
}

