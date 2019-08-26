
 /*
 * newtest.c
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>


//#include "clk.h"
//#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "version.h"
#include "ws2811.h"*/
#include <libraries/demo_light/lightsys.h>


// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                19				/// select the GPIO pin to send data. 
#define DMA                     10				/// select DMA channel to use 
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)


#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

#define WIDTH                   4	/// number of lights
#define HEIGHT                  1	/// in case of light matrix: number of lights rows
#define LED_COUNT               (WIDTH * HEIGHT)

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = 0,//GPIO_PIN,
            .count = 0,//LED_COUNT,
            .invert = 0,
            .brightness = 0,//255,
	    .strip_type = STRIP_TYPE,
        },
        [1] =
        {
            .gpionum = GPIO_PIN, //.gpionum = 0,
            .count = LED_COUNT, //.count = 0,
            .invert = 0,
            .brightness = 255, //0,
	    .strip_type = STRIP_TYPE, // this line shall be deleted
        },
    },
};

ws2811_led_t *matrix;

/// Define colors
ws2811_led_t	blue	= 0xf00000;
ws2811_led_t	red 	= 0x00f000;
ws2811_led_t	red_lite= 0x002000;
ws2811_led_t	green 	= 0x0000f0;
ws2811_led_t	white 	= 0xf0f0f0;
ws2811_led_t	yellow	= 0x00f090;
ws2811_led_t	orange 	= 0x00a020;
ws2811_led_t	purple 	= 0x50a000;
ws2811_led_t	pink 	= 0x20c020;
ws2811_led_t	off 	= 0x000000;
/// define temps for saving colors
ws2811_led_t	colortemp1= 0;
ws2811_led_t	colortemp2= 0;

int clear_on_exit = 0;

/// define each light position regarding its position in series connection
#define	Light_F_R	matrix[0] 	///Front Right light
#define	Light_F_L	matrix[1]	///Front Left light
#define	Light_R_R	matrix[3]	///Rear Right light
#define	Light_R_L	matrix[2]	///Rear Left light

static uint8_t running = 1;		/// Key to stop the lighting loops

/// assign the matrix values to final  light data matrix
void matrix_render(void)
{
    int x, y;

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            ledstring.channel[1].leds[(y * width) + x] = matrix[y * width + x];
        }
    }
}
void lightsys_initial(void)
{
    ws2811_return_t ret;
    matrix = malloc(sizeof(ws2811_led_t) * width * height);
    setup_handlers();

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
	//return ret;
    }
}

void matrix_clear(void)
{
    int x, y;

    for (y = 0; y < (height ); y++)
    {
        for (x = 0; x < width; x++)
        {
            matrix[y * width + x] = 0;
        }
    }
}


/// to stop lighting loops by keyboard
static void ctrl_c_handler(int signum)
{
	(void)(signum);
    running = 0;
}


void setup_handlers(void)//static void setup_handlers(void)
{
    struct sigaction sa =
    {
        .sa_handler = ctrl_c_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
} 

/*
 *
 * Application API Functions
 *
 */


/**
 * Allocate and initialize memory, buffers, pages, PWM, DMA, and GPIO.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  0 on success, -1 otherwise.
 */

/// function to turn on the lights
void light_on(void)
{
    ws2811_return_t ret;
       
	Light_F_R = white;
	Light_F_L = white;
	Light_R_R = red_lite;
	Light_R_L = red_lite;
	matrix_render();
	
        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render_light_on failed: %s\n", ws2811_get_return_t_str(ret));
           
        }
            
}

///turn off the lights
void light_off(void)
{
    ws2811_return_t ret;
       
	Light_F_R = off;
	Light_F_L = off;
	Light_R_R = off;
	Light_R_L = off;
	matrix_render();
	
        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render_light_on failed: %s\n", ws2811_get_return_t_str(ret));
           
        }

        
    
}

/// backward drive lights
void light_BackW (void)
{
    ws2811_return_t ret;
   
	colortemp1 = Light_R_R;
	colortemp2 = Light_R_L;       
	
	Light_R_R = yellow;
	Light_R_L = yellow;
	matrix_render();
	
        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render_light_on failed: %s\n", ws2811_get_return_t_str(ret));
           
        }
    
}




void light_Blink_R(void)
{
    
    ws2811_return_t ret;
    
    // save the last colors before blinking
    colortemp1 = Light_F_R;
    colortemp2 = Light_R_R;
    running = 1;
    for(int i=0;i<10;i++)
    {
    	if (!running) break;
	Light_F_R = orange;
	//Light_F_L = off;
	Light_R_R = orange;
	//Light_R_L = off;
	matrix_render();
	
	if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        }
	usleep(1000000 / 2);
		
	Light_F_R = colortemp1;
	//Light_F_L = off;
	Light_R_R = colortemp2;
	//Light_R_L = off;
	matrix_render();
	
	if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        }
	usleep(1000000 / 2);
	

    }
    
}


void light_Blink_L(void)
{
    
    ws2811_return_t ret;
    
    // save the last colors in temps
    colortemp1 = Light_F_L;
    colortemp2 = Light_R_L;
    running = 1;
      for(int i=0;i<10;i++)
    {
    	if (!running) break;
	Light_F_L = orange;
	Light_R_L = orange;
	
	matrix_render();
	
	if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        }
	usleep(1000000 / 2);
		
	Light_F_L = colortemp1;
	Light_R_L = colortemp2;
	
	matrix_render();
	
	if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        }
	usleep(1000000 / 2);
	
	
       
    }
    
}


