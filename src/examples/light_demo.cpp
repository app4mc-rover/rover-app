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

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "version.h"
#include "ws2811.h"*/

#include <stdio.h>
#include <unistd.h>

//lightsystem
#include <roverapi/rover_light.hpp>

//Basis Include
#include <roverapi/rover_api.hpp>


using namespace rover;

int main()//int main(int argc, char *argv[])
{
    // light system initialization
    /*ws2811_return_t ret;
    matrix = malloc(sizeof(ws2811_led_t) * width * height);
    setup_handlers();

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }*/
	printf("Started roverapp light example.\n");
	
	//light system initial
	RoverLight r_light = RoverLight();
	r_light.initialize();
	    
	//This initialization is a one time only must-call before every rover application.
	RoverBase r_base = RoverBase();
	
	//Action
	fprintf(stderr, "Lights on for 3 Sec.\n");
	r_light.on();	
	sleep (3);
	r_light.off();
	fprintf(stderr, "Lights turn right for 10 Sec. Ctrl+C to finish this step.\n");
	//stop_blink();
	r_light.Blink_R();
	//stop_blink();
	fprintf(stderr, "Lights turn left for 10 Sec. Ctrl+C to finish this step.\n");	
	r_light.Blink_L();
	//stop_blink();
	fprintf(stderr, "Lights backward for 3 Sec.\n");
	r_light.BackW();
	sleep (3);
	fprintf(stderr, "Lights off\n");
	r_light.off();
	fprintf(stderr, "THE END.\n");
	
	

    

    printf ("\n");
    //return ret;
}
