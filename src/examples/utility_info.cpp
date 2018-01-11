/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover utility info example -
 *        Used for reading status and core usage information from the rover
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//Utils Include
#include <roverapi/rover_utils.hpp>

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started roverapp utility_info example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
     // Instantiate a RoverUtils object to access member functions that deals with status and performance tasks
    RoverUtils r_utils = RoverUtils(); //RoverUtils does not need to be initialized
    
    // Print core utilization from the rover's OS
    float util[4];
    r_utils.getCoreUtilization(util);
    printf ("Utilization = [%f %f %f %f]\n", util[0], util[1], util[2], util[3]);

	printf("Exiting.\n");

	return 0;
}