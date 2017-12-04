/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Driving API - Interfaces for Rover driving application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_driving.hpp>
#include <roverapi/basic_psys_rover.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>

rover::RoverDriving::RoverDriving()
:SPEED(HIGHEST_SPEED),
 ROVERDRIVING_INIT_(0)
{

}
void rover::RoverDriving::setSpeed (const int speed_setpoint)
{
	this->SPEED = speed_setpoint;
}

int rover::RoverDriving::getSpeed (void)
{
	return this->SPEED;
}

void rover::RoverDriving::initialize (void)
{
	//wiringPiSetup();
	/* Initialize RoverDriving once */
	pinMode (ENABLE_MOTOR_LEFT, OUTPUT) ;
	digitalWrite (ENABLE_MOTOR_LEFT, HIGH) ;
	pinMode (ENABLE_MOTOR_RIGHT, OUTPUT) ;
	digitalWrite (ENABLE_MOTOR_RIGHT, HIGH) ;
	pinMode (DIRECTION_PIN_LEFT, OUTPUT) ;
	pinMode (DIRECTION_PIN_RIGHT, OUTPUT) ;


	softPwmCreate (SOFT_PWM_ENGINE_LEFT, 0, HIGHEST_SPEED) ;
	softPwmCreate (SOFT_PWM_ENGINE_RIGHT, 0, HIGHEST_SPEED) ;

	pinMode (FLASH_LIGHT_LED, OUTPUT) ;

	this->ROVERDRIVING_INIT_ = 1;
}

void rover::RoverDriving::turnForwardRight (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turn(FORWARD, LEFT, this->SPEED);
	}
}

void rover::RoverDriving::turnForwardLeft (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turn(FORWARD, RIGHT, this->SPEED);
	}
}

void rover::RoverDriving::turnBackwardRight (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turn(BACKWARD, LEFT, this->SPEED);
	}
}

void rover::RoverDriving::turnBackwardLeft (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turn(BACKWARD, RIGHT, this->SPEED);
	}
}

void rover::RoverDriving::turnLeft (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turnOnSpot(FORWARD, LEFT, this->SPEED);
	}
}

void rover::RoverDriving::turnRight (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		turnOnSpot(FORWARD, RIGHT, this->SPEED);
	}
}

void rover::RoverDriving::goForward (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		go(FORWARD, this->SPEED);
	}
}

void rover::RoverDriving::goBackward (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		go(BACKWARD, this->SPEED);
	}
}

void rover::RoverDriving::stopRover (void)
{
	if (this->ROVERDRIVING_INIT_ != 1)
	{
		fprintf(stderr,"You havent initialized RoverDriving. Use RoverDriving()::initialize() !\n");
	}
	else
	{
		stop();
	}
}
