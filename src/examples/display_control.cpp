/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover OLED Display control example -
 *        Displays the internet status on the OLED display
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//Display Include
#include <roverapi/rover_display.hpp>

//Utils Include
#include <roverapi/rover_utils.hpp>

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started roverapp display example.\n");
    
    RoverUtils r_utils = RoverUtils();
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
    // Use the OLED display on the rover
    RoverDisplay my_display = RoverDisplay();
    my_display.initialize();
    
    //--- Show a "Welcome to Rover" message
    printf("Showing a 'Welcome to Rover' message\n");
	// Prepare display contents
	my_display.clearDisplay();
	my_display.setTextSize(2);
	my_display.setTextColor(my_display.WHITE_COLOR);
	my_display.setCursor(10,10);
	my_display.print("Welcome");
	my_display.setTextSize(2);
	my_display.setTextColor(my_display.WHITE_COLOR);

	my_display.setCursor(10,32);
	my_display.print("to Rover");

	// Display now
	my_display.display();

	// Sleep for a bit
	r_base.sleep (2000);

    //--- Show internet status
	printf("Showing internet status\n");
    // Prepare display contents
    my_display.clearDisplay();
    my_display.setTextSize(2);
    my_display.setTextColor(my_display.WHITE_COLOR);
    my_display.setCursor(12,10);
    my_display.print("INTERNET:");
    my_display.setTextSize(3);
    my_display.setTextColor(my_display.WHITE_COLOR);
    
    // Check if internet is connected on the rover
    if (r_utils.getInternetStatus() == 1)
    {
        my_display.setCursor(50,32);
        my_display.print("ON");
    }
    else
    {
        my_display.setCursor(43,32);
        my_display.print("OFF");
    }
    

    // Display now
	my_display.display();

	printf("Exiting.\n");

	return 0;
}
