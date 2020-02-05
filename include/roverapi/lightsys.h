/*
 * Copyright (c) 2016 Eclipse Foundation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Authors:
 *    Mobin Pourreza; mobin.pourreza@gmail.com
 * Contributors:
 */

#ifndef __lightsys_H__
#define __lightsys_H__

#ifdef __cplusplus
extern "C" {
#endif


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

#include <libraries/lightsys/dma.h>
#include <libraries/lightsys/pwm.h>
#include <libraries/lightsys/version.h>
#include <libraries/lightsys/ws2811.h>

extern int blink_count;
extern int width;
extern int height;
extern int led_count;
extern ws2811_t ledstring;


ws2811_led_t *matrix;

static void ctrl_c_handler(int signum);
void setup_handlers(void);
ws2811_return_t initial(void);
void light_on(void);
void light_off(void);
void light_BackW (void);
void light_Blink_R(void);
void light_Blink_L(void);
void stop_blink(void);
void light_Blink_R_on(void);
void light_Blink_L_on(void);
void light_dim(int);





#ifdef __cplusplus
}
#endif

#endif /* __lightsys_H__ */
