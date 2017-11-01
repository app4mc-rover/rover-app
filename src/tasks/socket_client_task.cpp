/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *     Socket client for rover-app to rover-web communication
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 26.10.2017
 *
 */

#include <tasks/socket_client_task.h>

#include <wiringPi.h>
#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <roverapp.h>

#include <unistd.h>
#include <string.h>

/* Signal header */
#include <signal.h>

/* Socket defs header */
#include <socket_settings.h>

/* json-cpp library */
#include <json/json.h>

#include <math.h>

/* Global definitions */
int roverapp_send_sockfd;

/* If returns nonzero -> socket has a non zero error status */
int checkClientSocketStatus (int socket_fd)
{
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
	return error;
}

Json::Value constructJSONData (int data_type)
{
	Json::Value data;
	switch (data_type)
	{
		case SENSOR_DATA:
			data["rover_dtype"] = "sensor";
			data["data"]["infrared0"] = ceil(infrared_shared[0]);
			data["data"]["infrared1"] = ceil(infrared_shared[1]);
			data["data"]["infrared2"] = ceil(infrared_shared[2]);
			data["data"]["infrared3"] = ceil (infrared_shared[3]);
			data["data"]["front"] = ceil (distance_sr04_front_shared);
			data["data"]["rear"] = ceil (distance_sr04_back_shared);
			data["data"]["temperature"] = ceil (temperature_shared);
			data["data"]["humidity"] = ceil (humidity_shared);
			data["data"]["bearing"] = ceil (bearing_shared);
			break;
		case UTIL_DATA:
			data["rover_dtype"] = "util";
			data["data"]["core0"] = cpu_util_shared[0];
			data["data"]["core1"] = cpu_util_shared[1];
			data["data"]["core2"] = cpu_util_shared[2];
			data["data"]["core3"] = cpu_util_shared[3];
			break;
		default:
			data["rover_dtype"] = "none";
			break;
	}
	return data;
}

void Socket_Client_Task_Terminator (int dummy)
{
	close(roverapp_send_sockfd);
	running_flag = 0;
}

void *Socket_Client_Task (void * arg)
{
	timing socket_client_task_tmr;

	CollectThreadName("Socket_Client_Task");

	socket_client_task_tmr.setTaskID("Socket_Client_Task");
	socket_client_task_tmr.setDeadline(0.8);
	socket_client_task_tmr.setPeriod(0.8);

	/* Add termination signal handler to properly close socket */
	signal(SIGINT, Socket_Client_Task_Terminator);
	signal(SIGTERM, Socket_Client_Task_Terminator);
	signal(SIGKILL, Socket_Client_Task_Terminator);

	/* Socket related initializations */
	int n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char client_buffer[JSON_DATA_BUFSIZE];
	char * eof_str = "\r\n";
	int connected_flag = 0;

	/* Send these data each iteration*/
	int job_count = 2;
	int job_list[job_count] = {SENSOR_DATA, UTIL_DATA};
	int job_idx = 0;

	/* Create JSON object to hold sensor data */
	Json::FastWriter  string_writer;

	while (running_flag)
	{
		socket_client_task_tmr.recordStartTime();
		socket_client_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
		/* For Connecting / Reconnecting */
		if (checkClientSocketStatus(roverapp_send_sockfd)!=0)
		{
			connected_flag = 0;
		}

		if (connected_flag == 0)
		{
			/* Try to connect or reconnect */
			/* Create socket, and connect to the socket */
			close(roverapp_send_sockfd);
			roverapp_send_sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (roverapp_send_sockfd < 0)
				perror("ERROR opening socket");
			server = gethostbyname(CONNECTION_HOSTNAME);
			if (server == NULL) {
				fprintf(stderr,"ERROR, no such host\n");
				exit(0);
			}
			bzero((char *) &serv_addr, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			bcopy((char *)server->h_addr,
				 (char *)&serv_addr.sin_addr.s_addr,
				 server->h_length);
			serv_addr.sin_port = htons(ROVERAPP_SEND_PORT);

			/* Try to connect */
			if (connect(roverapp_send_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) == 0)
			{
				connected_flag = 1;
				job_idx = 0;
			}
		}
		else
		{
			/* Connected, start writing */
			/* Construct the JSON data and Write to string */
			std::string temp = string_writer.write(constructJSONData(job_list[job_idx++]));
			if (job_idx == job_count)
				job_idx = 0;

			/* Clear buffer and Write to buffer from string */
			bzero(client_buffer, JSON_DATA_BUFSIZE);
			if (strncpy(client_buffer, temp.c_str(), sizeof(client_buffer)) == NULL)
			{
				perror("strcpy temp");
			}
			client_buffer[sizeof(client_buffer)-1] = 0;

			/* Attach end of file character to string*/
			if (strcat(client_buffer, eof_str) == NULL)
			{
				perror("strcat EOF");
			}

			/* Write to socket */
			if (write(roverapp_send_sockfd, client_buffer, strlen(client_buffer)) == -1)
			{
				perror("write");
				/* Very important approach to have re-usable socket client */
				/* Checking connection status by using exit status of 'write' */
				printf ("Disconnected from server : socket_client.cpp");
				connected_flag = 0;
			}
		}

		//Task content ends here -------------------------------------------------
		socket_client_task_tmr.recordEndTime();
		socket_client_task_tmr.calculateExecutionTime();
		socket_client_task_tmr.calculateDeadlineMissPercentage();
		socket_client_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&socket_client_task_ti_l);
			socket_client_task_ti.deadline = socket_client_task_tmr.getDeadline();
			socket_client_task_ti.deadline_miss_percentage = socket_client_task_tmr.getDeadlineMissPercentage();
			socket_client_task_ti.execution_time = socket_client_task_tmr.getExecutionTime();
			socket_client_task_ti.period = socket_client_task_tmr.getPeriod();
			socket_client_task_ti.prev_slack_time = socket_client_task_tmr.getPrevSlackTime();
			socket_client_task_ti.task_id = socket_client_task_tmr.getTaskID();
			socket_client_task_ti.start_time = socket_client_task_tmr.getStartTime();
			socket_client_task_ti.end_time = socket_client_task_tmr.getEndTime();
		pthread_mutex_unlock(&socket_client_task_ti_l);
		socket_client_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}



