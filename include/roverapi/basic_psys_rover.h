/*
 * Copyright (c) 2016 Eclipse Foundation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Authors:
 *    Gaël Blondelle - initial API and parameters
 * Contributors:
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <wiringPi.h>

#ifndef PSYS_ROVER_H_
#define PSYS_ROVER_H_

#define FORWARD 	1
#define BACKWARD	-1
#define LEFT	1
#define RIGHT	2
#define DEFAULT_DELAY	450

#define FULL_SPEED	480
#define LOW_SPEED	360

#define	DIRECTION_PIN_LEFT		5 // Physical 18
#define SOFT_PWM_ENGINE_LEFT 	26 // Physical 32
#define ENABLE_MOTOR_LEFT		3 // Physical 15
#define	DIRECTION_PIN_RIGHT		6 // Physical 22
#define SOFT_PWM_ENGINE_RIGHT 	23 // Physical 33
#define ENABLE_MOTOR_RIGHT		4 // Physical 16
#define FLASH_LIGHT_LED			7 // Physical 7

#define LED_FLEFT				4 // Physical 16
#define LED_FRIGHT				5 // Physical 18
#define DISTANCE				6 // Physical 22

// Analog sensors
#define BASE 200
#define SPI_CHAN 0

extern void init(void);

extern void stop(void);

extern void shutdown_rover(void);

extern void go(int direction, int speed);

extern void turn(int direction, int side, int speed);

extern void turnOnSpot (int direction, int side, int speed);

extern void toggle_light(void);

extern float getDistance(int channel);

extern float getTemperature(void);

extern float getHumidity(void);

extern void runside(int side, int direction, int speed);

extern void differential_drive( float angular_speed , float linear_speed );


#endif /* PSYS_ROVER_H_ */



#ifdef __cplusplus
}
#endif
