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


#include <roverapi/rover_light.hpp>
#include <roverapi/lightsys.h>

#include <stdio.h>
#include <unistd.h>

using namespace std;



rover::RoverLight::RoverLight()
{
    ROVERLIGHT_INIT_ = 0;
    dimValue = 0xf0;
}

rover::RoverLight::~RoverLight()
{
    if (ROVERLIGHT_INIT_)
    {
        this->off();
        ws2811_fini(&ledstring);

    }
}




void rover::RoverLight::initialize(void)
{
    /* Initialize RoverLight once */
    ws2811_return_t ret;
    ret = initial();

    if ( ret != WS2811_SUCCESS)
    {
        fprintf(stderr, "light initial failed: %s\n", ws2811_get_return_t_str(ret));	
    }
    else
    {
        this->ROVERLIGHT_INIT_ = 1;
        fprintf(stderr, "light initialized successfully: %s---%d\n", ws2811_get_return_t_str(ret),this->ROVERLIGHT_INIT_);
        
    }
}

void rover::RoverLight::intruptsetup(void)
{
        setup_handlers();
}

/// function to turn on the lights
void rover::RoverLight::on(void)
{

    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {
        light_on();
     }       
}

///turn off the lights
void rover::RoverLight::off(void)
{
 
    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    { 
        light_off();
    }   
    
}

/// backward drive lights
void rover::RoverLight::BackW (void)
{
        if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {
        light_BackW();
    }
}




void rover::RoverLight::Blink_R(void)
{
    
    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {
        light_Blink_R();        
    }
}


void rover::RoverLight::Blink_L(void)
{

    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {    
       light_Blink_L();       
    }
    
}

void rover::RoverLight::Blink_R_on(void)
{
    
    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {
        light_Blink_R_on();        
    }
}


void rover::RoverLight::Blink_L_on(void)
{

    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {    
       light_Blink_L_on();       
    }
    
}


void rover::RoverLight::dim(void)
{

    if (this->ROVERLIGHT_INIT_ != 1)
    {
        fprintf(stderr,"You havent initialized RoverLight. Use RoverLight::initialize() !\n");
    }
    else
    {    
       light_dim(dimValue); 
       fprintf(stderr,"dim is called from RoverLight object %d\n", dimValue);      
    }
    
}

int rover::RoverLight::dimget(void)
{

    return dimValue;
    
}

void rover::RoverLight::dimset(int dim)
{

    dimValue = dim;
    
}
