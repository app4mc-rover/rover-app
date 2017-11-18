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
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_DISPLAY_HPP_
#define API_ROVER_DISPLAY_HPP_

#include <roverapi/rover_api.hpp>

namespace rover
{
	class RoverDisplay : public RoverBase
	{
		public:
			void begin (void);
			void display (void);
			void clearDisplay (void) ;
			void drawBitmap (int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) ;
			void setCursor (int16_t x, int16_t y);
			void setTextSize (uint8_t s);
			void setTextColor (uint16_t c);
			void setTextColor (uint16_t c, uint16_t b);
			void printf (const char * format, ...);
			void print (const char * string);
			void drawRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	};
}



#endif /* API_ROVER_DISPLAY_HPP_ */
