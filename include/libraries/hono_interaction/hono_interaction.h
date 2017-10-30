/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Hono interaction library for Rover - Header file
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 */

#ifndef HONO_INTERACTION_HONO_INTERACTION_H_
#define HONO_INTERACTION_HONO_INTERACTION_H_


//Defines

//#define DEBUG_HTTP_RESPONSE 1
#define DEBUG_DEVICE_REGISTRATION 1


//Interfaces

int registerEntriesToHonoInstance (void);
int registerDeviceToHonoInstance (char * host_name, int port, char * tenant_name, char * device_id);
int sendTelemetryDataToHonoInstance (char * host_name, int port, char * tenant_name, char * device_id, char * field, double value);
int sendEventDataToHonoInstance (char * host_name, int port, char * tenant_name, char * device_id, char * field, double value);
int handleCode(int code);

#endif /* HONO_INTERACTION_HONO_INTERACTION_H_ */
