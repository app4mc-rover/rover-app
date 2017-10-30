/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     Socket communication ports, bufsizes, and other definitions
 *     implemented for Rover
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 26.10.2017
 *
 */


#ifndef SOCKET_SETTINGS_H_
#define SOCKET_SETTINGS_H_

/* Socket related */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//Data type definitions
#define SENSOR_DATA 			0
#define UTIL_DATA 				1
#define CONTROL_DATA 			2
#define SPEED_DATA 				3

/* Hostname to be used */
#define LOCAL_HOSTNAME			((const char*)"127.0.0.1")
#define BINDING_HOSTNAME		LOCAL_HOSTNAME
#define CONNECTION_HOSTNAME		LOCAL_HOSTNAME

/* Ports allocated for inter-process communication with the web-page */
#define ROVERAPP_LISTEN_PORT 	((int) 8501)
#define ROVERAPP_SEND_PORT 		((int) 8502)
/* Other way around for rover-web: */
//#define ROVERWEB_SEND_PORT 	((int) 8501)
//#define ROVERWEB_LISTEN_PORT 	((int) 8502)

/* Buffer size to different interfaces */
#define JSON_DATA_BUFSIZE		((int) 512)

/* End of file character */
#define CARRIAGE_RETURN 		'\r'
#define LINE_FEED       		'\n'
#define EOF_STR         		"\r\n"

#endif /* SOCKET_SETTINGS_H_ */
