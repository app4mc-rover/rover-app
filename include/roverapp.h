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

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <roverapi/rover_api.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_utils.hpp>

using namespace rover;

// Use the following when cross compiling with Eclipse CDT SDK
//#define CROSS_COMPILE_ECLIPSE 1

#if SIMULATOR
#define DEBUG_WO_RSL 1
#endif

// When debugging without RoverSenseLayer uncomment the following to prevent OS
// to shutdown.
//#define DEBUG_WO_RSL 1

// HMC5883L or QMC5883L ?
//#define USE_HMC5883L 1

// Please comment the line below to work with SR-04 sensor instead of GROOVE for
// rear proximity sensing.
//#define USE_GROOVE_SENSOR 1

/* For proper termination */
extern RoverBase r_base;
extern RoverDriving r_driving;
extern RoverDisplay my_display;
extern RoverUtils r_utils;

#endif /* ROVERAPP_H_ */
