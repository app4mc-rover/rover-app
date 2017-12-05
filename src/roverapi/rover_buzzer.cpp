/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    RoverBuzzer API - Interfaces for Rover buzzer application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created C++API 05.12.2017
 *
 */


#include <roverapi/rover_buzzer.hpp>
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

rover::RoverBuzzer::RoverBuzzer()
:BUZZER_FREQUENCY(200), /* Initialize default buzzer frequency */
 ROVERBUZZER_INIT_(0),
 BUZZER_PIN (DEFAULT_BUZZER_PIN)
{

}

void rover::RoverBuzzer::initialize (void)
{
	//wiringPiSetup();
	/* Setup buzzer */
	this->wPiPinMode (this->BUZZER_PIN, this->SOFT_TONE_OUTPUT_);
	this->wPiSoftToneCreate (this->BUZZER_PIN);

	this->ROVERBUZZER_INIT_ = 1;
}

void rover::RoverBuzzer::setBuzzerFrequency (const int buzzer_freq)
{
	this->BUZZER_FREQUENCY = buzzer_freq;
}

int rover::RoverBuzzer::getBuzzerFrequency (void)
{
	return this->BUZZER_FREQUENCY;
}

void rover::RoverBuzzer::setBuzzerOn (void)
{
	if (this->ROVERBUZZER_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		this->wPiSoftToneWrite (this->BUZZER_PIN, this->BUZZER_FREQUENCY);
	}
}

void rover::RoverBuzzer::setBuzzerOff (void)
{
	if (this->ROVERBUZZER_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		this->wPiSoftToneWrite (this->BUZZER_PIN, 0);
	}
}

void rover::RoverBuzzer::shutdownTone (void)
{
	if (this->ROVERBUZZER_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		this->wPiSoftToneWrite (this->BUZZER_PIN, 300);
		this->wPiDelay (2000);
		this->wPiSoftToneWrite (BUZZER_PIN, 0);
	}
}

void rover::RoverBuzzer::setBuzzerTone (const int buzzer_freq)
{
	if (this->ROVERBUZZER_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverGpio. Use RoverGpio()::initialize() !\n");
	}
	else
	{
		this->wPiSoftToneWrite (this->BUZZER_PIN, buzzer_freq);
	}
}

void rover::RoverBuzzer::setBuzzerPin (const int pin)
{
	this->BUZZER_PIN = pin;
}


