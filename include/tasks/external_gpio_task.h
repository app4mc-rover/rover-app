/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    External GPIO task for handling buttons and buzzer for Rover - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 11.10.2017
 *
*/

#ifndef TASKS_EXTERNAL_GPIO_TASK_H_
#define TASKS_EXTERNAL_GPIO_TASK_H_

#include <stdio.h>
#include <stdlib.h>

/* Defines */

/* Buzzer */
#define BUZZER_PIN 28 //BCM-20, wiringpi 28
#define BUZZER_ON_FREQ 200
#define BUZZER_SHUTDOWN_FREQ 300
#define BUZZER_OFF_FREQ 0

/* Shutdown Button */
#define SHUTDOWN_BUTTON_PIN 24 //BCM-21, wiringpi 24

/* User Button */
#define USER_BUTTON_PIN 27 //BCM-16, wiringpi 27

/* Interfaces */
void *External_GPIO_Task(void * arg);
void turnBuzzerOn (void);
void turnBuzzerOff (void);
void playShutdownTone (void);
void playOffTone (void);
void shutdownOS (void);

#endif /* TASKS_EXTERNAL_GPIO_TASK_H_ */
