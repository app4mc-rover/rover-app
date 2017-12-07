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
	#if !SIMULATOR
	digitalWrite (pin, value);
	#endif
}

int rover::RoverGpio::wPiDigitalRead (const int pin)
{
	#if !SIMULATOR
	return digitalRead (pin);
	#endif
}

void rover::RoverGpio::wPiPinMode (const int pin, const int set_val)
{
	#if !SIMULATOR
	pinMode (pin, set_val);
	#endif
}

void rover::RoverGpio::wPiSoftToneCreate (const int pin)
{
	#if !SIMULATOR
	softToneCreate (pin);
	#endif
}

void rover::RoverGpio::wPiSoftToneWrite (const int pin, const int tone)
{
	#if !SIMULATOR
	softToneWrite (pin, tone);
	#endif
}

void rover::RoverGpio::wPiDelay (const unsigned int period_ms)
{
	delay (period_ms);
}
