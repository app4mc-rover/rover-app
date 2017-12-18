/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     RoverDHT22 API - Interfaces for Rover DHT22 temperature and humidity sensor application development
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created RoverDHT22 class 04.12.2017
 *
 */

#include <roverapi/rover_dht22.hpp>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

rover::RoverDHT22::RoverDHT22()
:DHT22Pin(DHT22_RPI_PIN),
 DHT22_SETUP_(0)
{

}

rover::RoverDHT22::~RoverDHT22(){}

void rover::RoverDHT22::initialize(void)
{
	this->DHT22_SETUP_ = 1;
}

float rover::RoverDHT22::read(void)
{
	if (this->DHT22_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverDHT22. Use RoverDHT22::initialize() !\n");
	}
	else
	{
		return this->readTemperature();
	}
}

float rover::RoverDHT22::readTemperature (void)
{
	uint8_t laststate;
	uint8_t counter;
	uint8_t j;
	uint8_t i;
	int data[5];

	int try_again;
	float f;
	float h;
	float c;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;

	f = 0.0;
	h = 0.0;
	c = 0.0;
	j = 0;
	i = 0;
	counter = 0;
	laststate = HIGH;
	try_again = 1;

#if SIMULATOR
	return 32.0;
#else
	if (this->DHT22_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverDHT22. Use RoverDHT22::initialize() !\n");
	}
	else
	{
		while (try_again == 1)
		{
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			data[4] = 0;

			f = 0.0;
			h = 0.0;
			c = 0.0;
			j = 0;
			i = 0;
			counter = 0;
			laststate = HIGH;

			/* pull pin down for 18 milliseconds */
			pinMode( this->DHT22Pin, OUTPUT );
			digitalWrite( this->DHT22Pin, LOW );
			delay( 18 );

			/* prepare to read the pin */
			pinMode( this->DHT22Pin, INPUT );

			/* detect change and read data */
			for ( i = 0; i < MAX_TIMINGS; i++ )
			{
				counter = 0;
				while ( digitalRead( this->DHT22Pin ) == laststate )
				{
					counter++;
					delayMicroseconds( 1 );
					if ( counter == 255 )
					{
						break;
					}
				}
				laststate = digitalRead( this->DHT22Pin );

				if ( counter == 255 )
					break;

				/* ignore first 3 transitions */
				if ( (i >= 4) && (i % 2 == 0) )
				{
					/* shove each bit into the storage bytes */
					data[j / 8] <<= 1;
					if ( counter > 16 )
						data[j / 8] |= 1;
					j++;
				}
			}

			delay (1000);

			/*
			 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
			 * print it out if data is good
			 */
			if ( (j >= 40) &&
				 (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
			{
				h = (float)((data[0] << 8) + data[1]) / 10;
				if ( h > 100 )
				{
					h = data[0];	// for DHT11
				}
				c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
				if ( c > 125 )
				{
					c = data[2];	// for DHT11
				}
				if ( data[2] & 0x80 )
				{
					c = -c;
				}
				f = c * 1.8f + 32;
	#ifdef DEBUG
				printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
	#endif
				try_again = 0;
			}
			else
			{
				/* Data not good */
				try_again = 1;
				//printf ("Data not good, skipping\n");

			}
		}

		/* Return temperature */
		return c;
	}

#endif
}

float rover::RoverDHT22::readHumidity (void)
{
	uint8_t laststate;
	uint8_t counter;
	uint8_t j;
	uint8_t i;
	int data[5];

	int try_again;
	float f;
	float h;
	float c;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;

	f = 0.0;
	h = 0.0;
	c = 0.0;
	j = 0;
	i = 0;
	counter = 0;
	laststate = HIGH;
	try_again = 1;

#if SIMULATOR
	return 40.0;
#else
	if (this->DHT22_SETUP_ != 1)
	{
		fprintf(stderr,"You havent set up RoverDHT22. Use RoverDHT22::initialize() !\n");
	}
	else
	{
		while (try_again == 1)
		{
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			data[4] = 0;

			f = 0.0;
			h = 0.0;
			c = 0.0;
			j = 0;
			i = 0;
			counter = 0;
			laststate = HIGH;

			/* pull pin down for 18 milliseconds */
			pinMode( this->DHT22Pin, OUTPUT );
			digitalWrite( this->DHT22Pin, LOW );
			delay( 18 );

			/* prepare to read the pin */
			pinMode( this->DHT22Pin, INPUT );

			/* detect change and read data */
			for ( i = 0; i < MAX_TIMINGS; i++ )
			{
				counter = 0;
				while ( digitalRead( this->DHT22Pin ) == laststate )
				{
					counter++;
					delayMicroseconds( 1 );
					if ( counter == 255 )
					{
						break;
					}
				}
				laststate = digitalRead( this->DHT22Pin );

				if ( counter == 255 )
					break;

				/* ignore first 3 transitions */
				if ( (i >= 4) && (i % 2 == 0) )
				{
					/* shove each bit into the storage bytes */
					data[j / 8] <<= 1;
					if ( counter > 16 )
						data[j / 8] |= 1;
					j++;
				}
			}

			delay(1000);

			/*
			 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
			 * print it out if data is good
			 */
			if ((j >= 40) &&
				 (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
			{
				h = (float)((data[0] << 8) + data[1]) / 10;
				if ( h > 100 )
				{
					h = data[0];	// for DHT11
				}
				c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
				if ( c > 125 )
				{
					c = data[2];	// for DHT11
				}
				if ( data[2] & 0x80 )
				{
					c = -c;
				}
				f = c * 1.8f + 32;
	#ifdef DEBUG
				printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
	#endif
				try_again = 0;
			}
			else
			{
				/* Data not good */
				try_again = 1;
				//printf ("Data not good, skipping\n");
			}
		}

		/* Return humidity */
		return h;
	}
#endif
}

void rover::RoverDHT22::setDHT22Pin (const int pin)
{
	this->DHT22Pin = pin;
}

int rover::RoverDHT22::getDHT22Pin (void)
{
	return this->DHT22Pin;
}


