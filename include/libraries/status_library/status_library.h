/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Status library for getting WiFi, Ethernet Interface, Internet, Bluetooth, Hono Cloud status
 *    header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 04.10.2017
 *
 */

#ifndef STATUS_LIBRARY_STATUS_LIBRARY_H_
#define STATUS_LIBRARY_STATUS_LIBRARY_H_

/* Interfaces */
int retrieveWLANStatus (void);
int retrieveETHStatus (void);
int retrieveINTERNETStatus (void);
int retrieveBLUETOOTHStatus (void);
int retrieveHONOStatus (char * host_name, int port, char * tenant_name, char * device_id, char * user, char * password);

#endif /* STATUS_LIBRARY_STATUS_LIBRARY_H_ */
