/*******************************************************************************
 * Copyright (c) 2017 FH-Dortmund and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     FH-Dortmund - initial API and implementation
 *******************************************************************************/

#ifndef ROVER_TEST_TASK_H_
#define ROVER_TEST_TASK_H_

//Includes needed
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <cstdio>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>
#include <libraries/timing/timing.h>
#include <roverapi/basic_psys_rover.h>
#include <interfaces.h>


void *Rover_Test_Task(void *unused);
void say(char *text);
void autopark() ;
void getDistanceTrain(int channel,int rounds);


#endif /* ROVER_TEST_TASK_H_ */
