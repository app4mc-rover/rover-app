/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Display API - Interfaces for Rover display application development - Header file
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

#ifndef API_ROVER_DISPLAY_HPP_
#define API_ROVER_DISPLAY_HPP_

#include <drivers/oled_drivers/Adafruit_SSD1306.h>

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

namespace rover
{
	/**
	 * @brief Contains the member functions to control OLED display on the Rover. This class is a wrapper API for Adafruit_GFX and Adafruit_SSD1306 libraries.
	 */
	class RoverDisplay
	{
		private:
			/**
			 * @brief Object to handle OLED operations.
			 */
			Adafruit_SSD1306 my_display;

			/**
			 * @brief Flag to hold if RoverDisplay is initialized.
			 */
			int ROVER_DISPLAY_INIT_;

		public:

			/**
			 * @brief Static definition to indicate BLACK color
			 */
			static const int BLACK_COLOR = 0;

			/**
			 * @brief Static definition to indicate WHITE color
			 */
			static const int WHITE_COLOR = 1;

			/**
			 * @brief (Default) Constructor for RoverDisplay class
			 */
			RoverDisplay();

			/**
			 * @brief Initializes the OLED display of the rover
			 * @return void
			 */
			void initialize (void);

			/**
			 * @brief Displays the buffer contents on the OLED display
			 * @return void
			 */
			void display (void);

			/**
			 * @brief Returns the display object
			 * @return void
			 */
			Adafruit_SSD1306& getDisplay (void);

			/**
			 * @brief Clears the display
			 * @return void
			 */
			void clearDisplay (void) ;

			/**
			 * @brief Draws a bitmap on the OLED display
			 * @param x is the horizontal pixel location to start drawing bitmap
			 * @param y is the vertical pixel location to start drawing bitmap
			 * @param w is the width of bitmap in pixels
			 * @param h is the height of bitmap in pixels
			 * @param color is the color to draw the bitmap. RoverDisplay::BLACK_COLOR or RoverDisplay::WHITE_COLOR
			 * @param bitmap is the unsigned integer array that contains the bitmap image.
			 * @return void
			 */
			void drawBitmap (int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) ;

			/**
			 * @brief Sets the cursor to a position in order to start typing text.
			 * @param x is the horizontal pixel location
			 * @param y is the vertical pixel location
			 * @return void
			 */
			void setCursor (int16_t x, int16_t y);

			/**
			 * @brief Sets the text size to one of the predefined sizes.
			 * @param s is the size in unsigned integer type. Example values involve 1,2,3.
			 * @return void
			 */
			void setTextSize (uint8_t s);

			/**
			 * @brief Sets the text color to either white or black
			 * @param c is the textcolor in integer. RoverDisplay::BLACK_COLOR or RoverDisplay::WHITE_COLOR
			 * @return void
			 */
			void setTextColor (uint16_t c);

			/**
			 * @brief Sets the text and background color color to either white or black
			 * @param c is the text color in integer. RoverDisplay::BLACK_COLOR or RoverDisplay::WHITE_COLOR
			 * @param b is the background color in integer. RoverDisplay::BLACK_COLOR or RoverDisplay::WHITE_COLOR
			 * @return void
			 */
			void setTextColor (uint16_t c, uint16_t b);

			/**
			 * @brief Prints a string to OLED display starting from cursor location
			 * @param string is the message to print to OLED display.
			 * @return void
			 */
			void print (const char * string);

			/**
			 * @brief Draws a rectangle to OLED display
			 * @param x is the horizontal pixel location to start drawing rectangle
			 * @param y is the vertical pixel location to start drawing rectangle
			 * @param w is the width of rectangle in pixels
			 * @param h is the height of rectangle in pixels
			 * @param color is the color to draw the rectangle. RoverDisplay::BLACK_COLOR or RoverDisplay::WHITE_COLOR
			 * @return void
			 */
			void drawRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

			/**
			 * @brief Draws a pixel
			 * @param x X position of the pixel
			 * @param y Y position of the pixel
			 * @param color Color of the pixel
			 */
			void drawPixel(int16_t x, int16_t y, uint16_t color);

			/**
			 * @brief Draws a circle
			 * @param x0 Center point X position
			 * @param y0 Center point Y position
			 * @param r Radius of the circle
			 * @param color Color of the circule
			 */
			void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

			/**
			 * @brief Draws a filled circle
			 * @param x0 Center point X position
			 * @param y0 Center point Y position
			 * @param r Radius of the circle
			 * @param color Color of the circule
			 */
			void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

			/**
			 * @brief Draws a line
			 * @param x0 Starting point X position
			 * @param y0 Starting point Y position
			 * @param x1 End point X position
			 * @param y1 End point Y position
			 * @param color Color of the line
			 */
			void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

			/**
			 * @brief Fills the entire screen
			 * @param color Color to fill the screen with
			 */
			void fillScreen(uint16_t color);

			/**
			 * @brief Draws a filled rounded rectangle
			 * @param x Rectangle location X
			 * @param y Rectangle location Y
			 * @param w Rectangle width
			 * @param h Rectangle height
			 * @param r Radius of the rounded corners
			 * @param color Color of the filled rounded rectangle.
			 */
			void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

			/**
			 * @brief Draws a rounded rectangle
			 * @param x Rectangle location X
			 * @param y Rectangle location Y
			 * @param w Rectangle width
			 * @param h Rectangle height
			 * @param r Radius of the rounded corners
			 * @param color Color of the rounded rectangle.
			 */
			void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

			/**
			 * @brief Draws a triangle
			 * @param x0 First point x location
			 * @param y0 First point y location
			 * @param x1 Second point x location
			 * @param y1 Second point y location
			 * @param x2 Third point x location
			 * @param y2 Third point y location
			 * @param color Color of the triangle
			 */
			void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

			/**
			 * @brief Draws a filled triangle
			 * @param x0 First point x location
			 * @param y0 First point y location
			 * @param x1 Second point x location
			 * @param y1 Second point y location
			 * @param x2 Third point x location
			 * @param y2 Third point y location
			 * @param color Color of the triangle
			 */
			void fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

			/**
			 * @brief Used to draw a character
			 * @param x X position of the character
			 * @param y Y position of the character
			 * @param bitmap Bitmap to draw
			 * @param w Width of the bitmap
			 * @param h Height of the bitmap
			 * @param color Color
			 */
			void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);

			/**
			 * @brief Used to rotate the display. Can be 0,1,2, or 3.
			 * @param x Rotation state: 0,1,2, or 3.
			 */
			void setRotation(uint8_t x);

			/**
			 * @brief Inverts the display
			 */
			void invertDisplay(boolean i);

			/**
			 * @brief Returns height of the display
			 */
			int16_t width(void);

			/**
			 * @brief Returns width of the display
			 */
			int16_t height(void);


	};
}



#endif /* API_ROVER_DISPLAY_HPP_ */
