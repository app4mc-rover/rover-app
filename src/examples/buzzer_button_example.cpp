/*
 * Copyright (c) 2018 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover buzzer and button example
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - created 17.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//Buzzer and button Include
#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_button.hpp>

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
	int millisecond = 0;

    printf("Started roverapp buzzer and button example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    // Instantiate buzzer and button
    RoverBuzzer r_buzzer = RoverBuzzer();
    RoverButton r_button = RoverButton(USER_BUTTON);

    //Initializations
    r_buzzer.initialize();
    r_button.initialize();

    // Check for a minute if button is pressed
    while (millisecond < 30000)
    {
		if (r_button.readButton() == r_button.LO) //LOW
		{
			/* Debounce */
			r_base.sleep(1);

			if (r_button.readButton() == r_button.LO)
			{
				/* Wait a bit more to prevent double pushes */
				r_base.sleep(400);

				/* Ring the buzzer */
				r_buzzer.setBuzzerFrequency (500); //0 to 1000
				r_buzzer.setBuzzerOn();

				r_base.sleep(2000); //Play for 2 seconds

				/* Turn off the buzzer */
				r_buzzer.setBuzzerOff();
			}
		}

		millisecond += 1;
		r_base.sleep(1);
	}



    
	printf("Exiting.\n");

	return 0;
}
