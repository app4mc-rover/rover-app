/*
 * Copyright (c) 2017 Fraunhofer IEM, Eclipse Foundation and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    HMC588L compass reading and calibration task (x,y plane) with with wiringPi and pThreads
 *
 * Authors:
 *     David Schmelter
 *     <david.schmelter@iem.fraunhofer.de>
 *
 * Contributors:
 *
 * Update History:
 *    22.03.2017 - initial revision
 *
 */

#ifndef COMPASS_SENSOR_TASK_H_
#define COMPASS_SENSOR_TASK_H_


#include <stdio.h>
#include <stdlib.h>



using namespace std;

void *CompassSensor_Task(void * arg);

#endif /* COMPASS_SENSOR_TASK_H_ */
