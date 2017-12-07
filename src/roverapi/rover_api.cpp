/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover API - Interfaces for basic Rover application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_api.hpp>
#include <wiringPi.h>

#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_sensor.hpp>
#include <roverapi/rover_honocloud.hpp>

/**
  *   @brief  Constructor for the RoverBase class
  */
rover::RoverBase::RoverBase()
:WIRINGPI_INIT_(0)
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
}

void rover::RoverBase::initializeWiringPi(void)
{
	/* wiringPi can only be called once per program, One solution: */
	static class Once { public: Once(){
		wiringPiSetup();
		printf("wiringPi Setup Done..\n");
		printf("RoverBase initialized..\n");
	}} Once_;
	this->WIRINGPI_INIT_ = 1;
}

void rover::RoverBase::shutdown (void)
{
	if (this->WIRINGPI_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized wiringPi library. Use RoverBase::initialize() !\n");
	}
	else
	{
		RoverDisplay my_display = RoverDisplay();
		RoverBuzzer r_buzzer = RoverBuzzer();
		r_buzzer.initialize();

		my_display.initialize();

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
		r_buzzer.shutdownTone();

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
	else
	{
		delay (period_ms);
	}
}

