/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Bluetooth RFCOMM communication task to read 'driving information' from connection
 *    (Adapted and improved upon http://people.csail.mit.edu/albert/bluez-intro/x502.html)
 *    For Eclipse APP4MC Rover
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 04.10.2017
 *    M.Ozcelikors <mozcelikors@gmail.com>, bluetooth setup instructions added 10.10.2017
 *
 * Usage Instructions:
 *    1) Set-up bluetooth on Raspberry Pi
 *         sudo apt-get install pi-bluetooth bluez bluez-tools blueman
 *         sudo systemctl start bluetooth
 *         Now, the following should work:
 *             hcitool scan
 *
 *    1.O) WARNING: OPTIONAL: Use them at your own RISK.. (ref: Raspberry Pi Forums)
 *         If the command above (hcitool scan) still do not work:
 *         Execute the following commands:
 *             sudo apt-get update
 *             sudo apt-get dist-upgrade
 *             sudo rm /etc/udev/rules.d/99-com.rules
 *             sudo apt-get -o Dpkg::Options::="--force-confmiss" install --reinstall raspberrypi-sys-mods
 *             (if the above does not work) sudo apt-get install raspberrypi-sys-mods (Enter Y to install rules)
 *             sudo systemctl reboot
 *             (Downgrading the kernel to 4.4.50)  sudo rpi-update 52241088c1da59a359110d39c1875cda56496764
 *
 *         Add the following to your /etc/rc.local and reboot:
 *             btuart
 *
 *         NOTE: This approach is only intended for Raspbian Jessie distributions that does not involve
 *               bluetooth installed. If you use the latest Raspbian Jessie distribution, you can skip
 *               these instructions. To check if the bluetooth is working, simply use following two
 *               commands:
 *                    service bluetooth status
 *                    hcitool scan
 *    2) Prepare libraries for cross compilation (if you use cross-compilation with Eclipse IDE)
 *         Please note that in order to use Bluetooth stack for the cross compilation on Raspberry Pi,
 *         download bluez stack and add the lib directory content to drivers/bluetooth_drivers/,
 *         with inclusion paths corrected.
 *         i.e. #include <bluetooth/bluetooth.h> -> #include "bluetooth.h"
 *
 *         This code is tested with bluez-5.9
 *         Download bluez-5.9/lib from: https://www.kernel.org/pub/linux/bluetooth/
 *
 */

#include <tasks/bluetooth_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
/* Important Note: RFCOMM socket type is supported by Android API, so we chose to use it */
#include <bluetooth/rfcomm.h>

#include <roverapp.h>

void *Bluetooth_Task (void * arg)
{
	timing bluetooth_task_tmr;
	bluetooth_task_tmr.setTaskID((char*)"BLE");
	bluetooth_task_tmr.setDeadline(1);
	bluetooth_task_tmr.setPeriod(1);

	/* Bluetooth Setup (RFCOMM socket, server) */
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buf[1024] = { 0 };
	int s, client, bytes_read;
	socklen_t opt = sizeof(rem_addr);

	/* To avoid getting error:taking address of temporary [-fpermissive] */
	//bdaddr_t bdaddr_temporary = {0xD8, 0x50, 0xE6, 0x30, 0xA3, 0xBE}; // Android device
	bdaddr_t bdaddr_temporary = {0, 0, 0, 0, 0, 0};

	/* Allocate socket */
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	/* Bind socket to port 1 of the first available local bluetooth adapter */
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = bdaddr_temporary; //*BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind(s, (struct sockaddr *) &loc_addr, sizeof(loc_addr));

	/* Put socket into listening mode */
	listen(s, 1);

	/* Accept one connection */
	client = accept(s, (struct sockaddr *) &rem_addr, &opt);

	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "Accepted connection from %s\n", buf);
	memset(buf, 0, sizeof(buf));

	/* Read data from the client */
	bytes_read = read(client, buf, sizeof(buf));
	if( bytes_read > 0 ) {
		printf("received [%s]\n", buf);
	}

	/* Close connection */
	close(client);
	close(s);

	while (running_flag.get())
	{
		bluetooth_task_tmr.recordStartTime();
		bluetooth_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------



		//Task content ends here -------------------------------------------------

		bluetooth_task_tmr.recordEndTime();
		bluetooth_task_tmr.calculateExecutionTime();
		bluetooth_task_tmr.calculateDeadlineMissPercentage();
		bluetooth_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&bluetooth_task_ti.mutex);
			bluetooth_task_ti.deadline = bluetooth_task_tmr.getDeadline();
			bluetooth_task_ti.deadline_miss_percentage = bluetooth_task_tmr.getDeadlineMissPercentage();
			bluetooth_task_ti.execution_time = bluetooth_task_tmr.getExecutionTime();
			bluetooth_task_ti.period = bluetooth_task_tmr.getPeriod();
			bluetooth_task_ti.prev_slack_time = bluetooth_task_tmr.getPrevSlackTime();
			bluetooth_task_ti.task_id = bluetooth_task_tmr.getTaskID();
			bluetooth_task_ti.start_time = bluetooth_task_tmr.getStartTime();
			bluetooth_task_ti.end_time = bluetooth_task_tmr.getEndTime();
		pthread_mutex_unlock(&bluetooth_task_ti.mutex);
		bluetooth_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
