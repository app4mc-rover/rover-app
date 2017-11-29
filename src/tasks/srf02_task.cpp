/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    SRF02 Proximity sensor Task using PThreads
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 05.10.2017
 *
 */

#include <tasks/srf02_task.h>

#include <wiringPi.h>
#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <roverapp.h>
#include <roverapi/basic_psys_rover.h>


/* Name of the I2C device we will be using */
/* For older raspberry pi modules use "/dev/i2c-0" instead of "/dev/i2c-1" for the i2c port */
const char *I2CDeviceName = "/dev/i2c-1";


unsigned int readFromSRF02 (int deviceAddr)
{
	/* Parameters for SRF02 */
	int fd; //File descriptor

	int  address = deviceAddr;
	unsigned char buf[10];  // Buffer for data being read/ written on the i2c bus

	unsigned int result;

	/* Open device for R/W operations */
	if ((fd = open(I2CDeviceName, O_RDWR)) < 0)
	{
		perror("This error occured");
		printf("Failed to open i2c port\n");
		exit(1);
	}

	/* Set the port options and set the address of the device we wish to speak to */
	if (ioctl(fd, I2C_SLAVE, address) < 0) {
		perror("This error occured");
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	/* Commands for performing a ranging */
	buf[0] = 0;
	buf[1] = 81;

	/* Write commands to the i2c port */
	if ((write(fd, buf, 2)) != 2)
	{
		perror("This error occured");
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	/* This sleep waits for the ping to come back */
	usleep(900000); //High-precision sleep system call!

	/* This is the register we wish to read from */
	buf[0] = 0;

	/* Send the register to read from */
	if ((write(fd, buf, 1)) != 1)
	{
		perror("This error occured");
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	if (read(fd, buf, 4) != 4)
	{
		/* Read back data into buf[] */
		perror("This error occured");
		printf("Unable to read from slave\n");
		exit(1);
	}
	else
	{
		unsigned char highByte = buf[2];
		unsigned char lowByte = buf[3];
		result = highByte;

		/* Calculate range as a word value */
		result <<= 8;
		result += lowByte;
		//printf("Device#%d: Software v: %d\n", deviceAddr, buf[0]);
		//printf("Device#%d: Range was: %u\n", deviceAddr, result);
	}

	close(fd);

	return result;
}


void *SRF02_Task (void * arg)
{
	timing srf02_task_tmr;
	srf02_task_tmr.setTaskID("SRF02");
	srf02_task_tmr.setDeadline(1);
	srf02_task_tmr.setPeriod(1);

	unsigned int front_sensor, rear_sensor;

	while (1)
	{
		srf02_task_tmr.recordStartTime();
		srf02_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		front_sensor = readFromSRF02 (FRONT_SRF02_ADDR);
		printf("FRONT SRF02: %u\n", front_sensor);
		rear_sensor =  readFromSRF02 (REAR_SRF02_ADDR);
		printf("REAR SRF02: %u\n", rear_sensor);

		//Task content ends here -------------------------------------------------

		srf02_task_tmr.recordEndTime();
		srf02_task_tmr.calculateExecutionTime();
		srf02_task_tmr.calculateDeadlineMissPercentage();
		srf02_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&srf02_task_ti_l);
			srf02_task_ti.deadline = srf02_task_tmr.getDeadline();
			srf02_task_ti.deadline_miss_percentage = srf02_task_tmr.getDeadlineMissPercentage();
			srf02_task_ti.execution_time = srf02_task_tmr.getExecutionTime();
			srf02_task_ti.period = srf02_task_tmr.getPeriod();
			srf02_task_ti.prev_slack_time = srf02_task_tmr.getPrevSlackTime();
			srf02_task_ti.task_id = srf02_task_tmr.getTaskID();
			srf02_task_ti.start_time = srf02_task_tmr.getStartTime();
			srf02_task_ti.end_time = srf02_task_tmr.getEndTime();
		pthread_mutex_unlock(&srf02_task_ti_l);
		srf02_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
