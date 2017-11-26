/*
 * Copyright (c) 2017 Eclipse Foundation, FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    pThread skeleton implementation for PolarSys rover
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *    Gael Blondelle - API functions
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *
 */

#ifndef ROVERAPP_H_
#define ROVERAPP_H_

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "interfaces.h"

#include <roverapi/rover_api.hpp>

//Using rover namespace from Rover API
using namespace rover;

extern RoverBase r;

extern pthread_t oled_thread;

#endif /* ROVERAPP_H_ */
