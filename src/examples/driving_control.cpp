/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover driving control example
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//Driving Include
#include <roverapi/rover_driving.hpp>

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started roverapp driving example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    // Driving with rover
    RoverDriving r_driving = RoverDriving();
    r_driving.initialize();
    r_driving.setSpeed(HIGHEST_SPEED);

    printf ("Going forward...\n");
    r_driving.goForward();
    r_base.sleep (1500); // Sleep for some time in milliseconds

    printf ("Turning right...\n");
    r_driving.turnRight();
    r_base.sleep (1500);

    printf ("Turning left...\n");
    r_driving.turnLeft();
    r_base.sleep (1500);

    printf ("Going backward...\n");
     r_driving.goBackward();
    r_base.sleep (1500);
    r_driving.stopRover();
    
	printf("Exiting.\n");

	return 0;
}
