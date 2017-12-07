/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     Socket server for rover-web to rover-app communication
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 26.10.2017
 *
 */

#include <tasks/socket_server_task.h>

#include <string.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <signal.h>

/* Socket defs header */
#include <socket_settings.h>

/* json-cpp library */
#if SIMULATOR
#include <jsoncpp/json/json.h>
#else
#include <json/json.h>
#endif

#include <roverapp.h>

/* Global definitions */
int roverapp_listen_sockfd;
int newroverapp_listen_sockfd;

/* If returns nonzero -> socket has a non zero error status */
int checkServerSocketStatus (int socket_fd)
{
	//TODO: Fix this function!
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
	return error;
}

void parseJSONData (char *server_buffer)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( server_buffer, root );
	if ( !parsingSuccessful )
	{
		std::cout  << "Failed to parse" << reader.getFormattedErrorMessages();
	}

	if (root["rover_dtype"].asString() == "control")
	{
			//Take only the first char - since interface uses 1 char only
			keycommand_shared = root["data"]["command"].asString()[0];
	}
	else if (root["rover_dtype"].asString() == "speed")
	{
			speed_shared = root["data"]["speed"].asInt();
	}
	else
	{
		std::cout << "Unable to parse in socket_server_task.cpp" << std::endl;
	}

}

void *Socket_Server_Task(void * arg)
{
	timing socket_server_task_tmr;

	socket_server_task_tmr.setTaskID("Socket_Server_Task");
	socket_server_task_tmr.setDeadline(0.05);
	socket_server_task_tmr.setPeriod(0.05);

	socklen_t clilen;
	char server_buffer[JSON_DATA_BUFSIZE];
	struct sockaddr_in serv_addr, cli_addr;
	int  n;
	int true_ = 1;
	int client_connected_flag = 0;

	/* First call to socket() function */
	roverapp_listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (roverapp_listen_sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	/* Handle re-binding problems */
	setsockopt(roverapp_listen_sockfd, SOL_SOCKET,SO_REUSEADDR,&true_,sizeof(int));

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(BINDING_HOSTNAME);
	serv_addr.sin_port = htons(ROVERAPP_LISTEN_PORT);
	memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);

	/* Now bind the host address using bind() call.*/
	if (bind(roverapp_listen_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

	/* Now start listening for the clients, here process will
	  * go in sleep mode and will wait for the incoming connection
	*/
	listen(roverapp_listen_sockfd,5); // Max 5 connections queued
	clilen = sizeof(cli_addr);

	while(running_flag.get())
	{

		socket_server_task_tmr.recordStartTime();
		socket_server_task_tmr.calculatePreviousSlackTime();

#if !SIMULATOR
		//Task content starts here -----------------------------------------------
		/* Handle client acception or re-acception */
		/* Checking if client socket is available */
		if (checkServerSocketStatus(newroverapp_listen_sockfd)!=0)
		{
			client_connected_flag = 0;
		}

		if (client_connected_flag == 0)
		{
			/* If no client is connected currently */
			/* Accept actual connection from the client */
			newroverapp_listen_sockfd = accept(roverapp_listen_sockfd, (struct sockaddr *)&cli_addr, &clilen);

			if (newroverapp_listen_sockfd < 0) {
				perror("ERROR on accept -> newroverapp_listen_sockfd");
				exit(1);
			}
			else
			{
				printf ("Client connected to newroverapp_listen_sockfd \n");
				client_connected_flag = 1;
			}
		}
		else
		{
			/* If a client is connected */
			/* If connection is established then start communicating */
			size_t buf_idx = 0;

			/* Read one char at a time until the EOF_STR "\r\n" is reached */
			while (buf_idx < JSON_DATA_BUFSIZE)
			{
				if ( 1 == read(newroverapp_listen_sockfd, &server_buffer[buf_idx], 1))
				{
					if (buf_idx > 0 &&  LINE_FEED == server_buffer[buf_idx] && CARRIAGE_RETURN == server_buffer[buf_idx - 1])
					{
						break;
					}
					buf_idx++;
				}
				else
				{
					/* Very important approach to have re-usable socket server */
					/* Client connection status is determined by exit status of read */
					printf ("Client disconnected!");
					client_connected_flag = 0;
					break;
				}

			}
			// Received buffer
			//printf ("buffer: %s\n",server_buffer);

			/* Remove \r\n from at the end of the string */
			server_buffer[buf_idx] = '\0';
			server_buffer[buf_idx-1] = '\0';

			/* Parse the JSON data, and update global variables */
			parseJSONData(server_buffer);
		}

#endif

		//Task content ends here -------------------------------------------------

		socket_server_task_tmr.recordEndTime();
		socket_server_task_tmr.calculateExecutionTime();
		socket_server_task_tmr.calculateDeadlineMissPercentage();
		socket_server_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&socket_server_task_ti.mutex);
			socket_server_task_ti.deadline = socket_server_task_tmr.getDeadline();
			socket_server_task_ti.deadline_miss_percentage = socket_server_task_tmr.getDeadlineMissPercentage();
			socket_server_task_ti.execution_time = socket_server_task_tmr.getExecutionTime();
			socket_server_task_ti.period = socket_server_task_tmr.getPeriod();
			socket_server_task_ti.prev_slack_time = socket_server_task_tmr.getPrevSlackTime();
			socket_server_task_ti.task_id = socket_server_task_tmr.getTaskID();
			socket_server_task_ti.start_time = socket_server_task_tmr.getStartTime();
			socket_server_task_ti.end_time = socket_server_task_tmr.getEndTime();
		pthread_mutex_unlock(&socket_server_task_ti.mutex);
		socket_server_task_tmr.sleepToMatchPeriod();

	}

	close(roverapp_listen_sockfd);
	close(newroverapp_listen_sockfd);

	return NULL;
}
