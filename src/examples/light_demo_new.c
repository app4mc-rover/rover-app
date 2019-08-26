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
#include <libraries/demo_light/lightsys.h>



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
	lightsys_initial();
	//Action
	fprintf(stderr, "Lights on for 3 Sec.\n");
	light_on();	
	usleep(3000000);
	fprintf(stderr, "Lights turn right for 10 Sec. Ctrl+C to finish this step.\n");
	light_Blink_R();
	fprintf(stderr, "Lights turn left for 10 Sec. Ctrl+C to finish this step.\n");
	light_Blink_L();
	fprintf(stderr, "Lights backward for 3 Sec.\n");
	light_BackW();
	usleep(3000000);
	fprintf(stderr, "Lights off\n");
	light_off();
	fprintf(stderr, "THE END.\n");
	

    ws2811_fini(&ledstring);

    printf ("\n");
    //return ret;
}
