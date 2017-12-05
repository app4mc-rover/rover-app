/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    RoverButton API - Interfaces for Rover button application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++API 05.12.2017
 *
 */

#include <roverapi/rover_button.hpp>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

rover::RoverButton::RoverButton (const RoverButtonType_t button_type)
{
	this->ROVERBUTTON_INIT_ = 0;

	if (button_type == USER_BUTTON)
	{
		this->buttonType = USER_BUTTON;
		this->buttonPin = USER_BUTTON_PIN;
	}
	else if (button_type == SHUTDOWN_BUTTON)
	{
		this->buttonType = SHUTDOWN_BUTTON;
		this->buttonPin = SHUTDOWN_BUTTON_PIN;
	}
	else if (button_type == CUSTOM_BUTTON)
	{
		this->buttonType = CUSTOM_BUTTON;
	}
}

void rover::RoverButton::initialize (void)
{
	if (this->buttonType == SHUTDOWN_BUTTON)
	{
		/* Setup shutdown button */
		this->wPiPinMode (this->SHUTDOWN_BUTTON_PIN, this->INPUT_);
	}
	else if (this->buttonType == USER_BUTTON)
	{
		/* Setup user button */
		this->wPiPinMode (this->USER_BUTTON_PIN, this->INPUT_);
	}
	else if (this->buttonType == CUSTOM_BUTTON)
	{
		/* Setup custom button */
		this->wPiPinMode (this->buttonPin, this->INPUT_);
	}

	this->ROVERBUTTON_INIT_ = 1;
}

int rover::RoverButton::readButton (void)
{
	if (this->ROVERBUTTON_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverButton. Use RoverButton()::initialize() !\n");
	}
	else
	{
		return this->wPiDigitalRead (this->buttonPin);
	}
}

void rover::RoverButton::setButtonPin (const int button_pin)
{
	this->buttonPin = button_pin;
}
