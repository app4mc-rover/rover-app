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

/* Interfaces */
void *External_GPIO_Task(void * arg);

#endif /* TASKS_EXTERNAL_GPIO_TASK_H_ */
