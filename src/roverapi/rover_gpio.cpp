/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Gpio API - Interfaces for Rover general purpose I/O application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_gpio.hpp>
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void rover::RoverGpio::initialize (void)
{
	//wiringPiSetup();
	/* Setup buzzer */
	pinMode (this->BUZZER_PIN, SOFT_TONE_OUTPUT);
	softToneCreate (this->BUZZER_PIN);

	/* Setup shutdown button */
	pinMode (this->SHUTDOWN_BUTTON_PIN, INPUT);

	/* Setup user button */
	pinMode (this->USER_BUTTON_PIN, INPUT);

	this->ROVERGPIO_INIT_ = 1;
}

rover::RoverGpio::RoverGpio()
:BUZZER_FREQUENCY(200), /* Initialize default buzzer frequency */
 ROVERGPIO_INIT_(0)
{

}

void rover::RoverGpio::setBuzzerFrequency (const int buzzer_freq)
{
	this->BUZZER_FREQUENCY = buzzer_freq;
}

int rover::RoverGpio::getBuzzerFrequency (void)
{
	return this->BUZZER_FREQUENCY;
}

void rover::RoverGpio::setBuzzerOn (void)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		softToneWrite (this->BUZZER_PIN, this->BUZZER_FREQUENCY);
	}
}

void rover::RoverGpio::setBuzzerOff (void)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		softToneWrite (this->BUZZER_PIN, 0);
	}
}

void rover::RoverGpio::shutdownTone (void)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		softToneWrite (this->BUZZER_PIN, 300);
		delay(2000);
		softToneWrite (BUZZER_PIN, 0);
	}
}

void rover::RoverGpio::setBuzzerTone (const int buzzer_freq)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		softToneWrite (this->BUZZER_PIN, buzzer_freq);
	}
}

int rover::RoverGpio::readUserButton (void)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		return digitalRead (this->USER_BUTTON_PIN);
	}
}

int rover::RoverGpio::readShutdownButton (void)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		return digitalRead (this->SHUTDOWN_BUTTON_PIN);
	}
}

void rover::RoverGpio::wPiDigitalWrite (const int pin, const int value)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		digitalWrite (pin, value);
	}
}

int rover::RoverGpio::wPiDigitalRead (const int pin)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		return digitalRead (pin);
	}
}

void rover::RoverGpio::wPiPinMode (const int pin, const int set_val)
{
	if (this->ROVERGPIO_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		pinMode (pin, set_val);
	}
}
