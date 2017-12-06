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


rover::RoverGpio::RoverGpio()
{

}

void rover::RoverGpio::wPiDigitalWrite (const int pin, const int value)
{
	digitalWrite (pin, value);
}

int rover::RoverGpio::wPiDigitalRead (const int pin)
{
	return digitalRead (pin);
}

void rover::RoverGpio::wPiPinMode (const int pin, const int set_val)
{
	pinMode (pin, set_val);
}

void rover::RoverGpio::wPiSoftToneCreate (const int pin)
{
	softToneCreate (pin);
}

void rover::RoverGpio::wPiSoftToneWrite (const int pin, const int tone)
{
	softToneWrite (pin, tone);
}

void rover::RoverGpio::wPiDelay (const unsigned int period_ms)
{
	delay (period_ms);
}
