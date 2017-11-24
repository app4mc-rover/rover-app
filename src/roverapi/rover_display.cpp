/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Display API - Interfaces for Rover display application development
 *
 * Usage Instructions:
 *    1) One should set-up I2C on Raspberry Pi to run this script properly (taken from Adafruit):
 *           1.1) Install necessary modules:
 *                sudo apt-get install -y python-smbus
 *                sudo apt-get install -y i2c-tools
 *           1.2) In /etc/modules, add following lines:
 *           	  i2c-bcm2708
 *                i2c-dev
 *           1.3) In /etc/modprobe.d/raspi-blacklist.conf, uncomment possible following lines:
 *                blacklist spi-bcm2708
 *                blacklist i2c-bcm2708
 *           1.4) In /boot/config.txt, add following lines:
 *                dtparam=i2c1=on
 *                dtparam=i2c_arm=on
 *           1.5) Then reboot
 *                sudo reboot
 *           1.6) To test I2C and see the addresses,
 *                sudo i2cdetect -y 1
 *    2) For cross-compilation using Eclipse IDE, make sure to follow the instructions located under readme.txt files
 *       in drivers/oled_drivers:
 *    	    Please note that in order to use OLED SSD1306 with I2C,
 *		    one has to download bcm2835.c, bcm2835.h, and has access to current <linux/i2c-dev.h>.
 *		    In case the compiler complains, please also download the newest i2c-dev.h and put it in drivers/oled_drivers folder.
 *		    Download i2c-dev.h from: https://www.kernel.org/pub/linux/
 *		    Download bcm2835.c and bcm2835.h from: https://github.com/hallard/ArduiPi_SSD1306
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#include <roverapi/rover_display.hpp>

#include <drivers/oled_drivers/ArduiPi_SSD1306.h>
#include <drivers/oled_drivers/Adafruit_GFX.h>

void rover::RoverDisplay::initialize(void)
{
	/* Type to describe default options for the OLED initialization */
	struct s_opts
	{
		int oled;
		int verbose;
	};

	/* Default options for the OLED initialization. */
	s_opts local_opts = {
		OLED_ADAFRUIT_I2C_128x64,		// Default oled
		false							// Not verbose
	};

	/* I2C change parameters to fit to your LCD */
	if ( !this->my_display.init(OLED_I2C_RESET, local_opts.oled) )
		exit(EXIT_FAILURE);

	this->my_display.begin();
	this->my_display.clearDisplay();   // clears the screen and buffer
}

void rover::RoverDisplay::display(void)
{
	this->my_display.display();
}

void rover::RoverDisplay::clearDisplay(void)
{
	this->my_display.clearDisplay();
}

void rover::RoverDisplay::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
	this->my_display.drawBitmap(x, y, bitmap, w, h, color);
}

void rover::RoverDisplay::setCursor (int16_t x, int16_t y)
{
	this->my_display.setCursor (x, y);
}

void rover::RoverDisplay::setTextSize (uint8_t s)
{
	this->my_display.setTextSize (s);
}

void rover::RoverDisplay::setTextColor (uint16_t c)
{
	this->my_display.setTextColor (c);
}

void rover::RoverDisplay::setTextColor (uint16_t c, uint16_t b)
{
	this->my_display.setTextColor (c, b);
}

void rover::RoverDisplay::print (const char * string)
{
	this->my_display.print (string);
}

void rover::RoverDisplay::drawRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	this->my_display.drawRect (x, y, w, h, color);
}
