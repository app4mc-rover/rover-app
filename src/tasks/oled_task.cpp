/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    SSD1306 I2C OLED Display Task
 *
 * Contributors / Revision History:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created 04.10.2017
 *    M.Ozcelikors <mozcelikors@gmail.com>, application is enhanced (status, appstacle displays) 08.10.2017
 *
 * Usage Instructions:
 *    1) One should set-up I2C on Raspberry Pi to run this script properly (taken from Adafruit):
 *           1.1) Install necessary modules:
 *                sudo apt-get install -y python-smbus
 *                sudo apt-get install -y i2c-tools
 *           1.2) In /etc/modules, add following lines:
 *           	  i2c-bcm2708
 *                i2c-dev
 *           1.3) In /etc/modprobe.d/raspi-blacklist.conf, uncomment possible following lines:
 *                blacklist spi-bcm2708
 *                blacklist i2c-bcm2708
 *           1.4) In /boot/config.txt, add following lines:
 *                dtparam=i2c1=on
 *                dtparam=i2c_arm=on
 *           1.5) Then reboot
 *                sudo reboot
 *           1.6) To test I2C and see the addresses,
 *                sudo i2cdetect -y 1
 *    2) For cross-compilation using Eclipse IDE, make sure to follow the instructions located under readme.txt files
 *       in drivers/oled_drivers:
 *    	    Please note that in order to use OLED SSD1306 with I2C,
 *		    one has to download bcm2835.c, bcm2835.h, and has access to current <linux/i2c-dev.h>.
 *		    In case the compiler complains, please also download the newest i2c-dev.h and put it in drivers/oled_drivers folder.
 *		    Download i2c-dev.h from: https://www.kernel.org/pub/linux/
 *		    Download bcm2835.c and bcm2835.h from: https://github.com/hallard/ArduiPi_SSD1306
 */

#include <tasks/oled_task.h>

#include <ctime>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>
#include <signal.h>

/* APPSTACLE Project logo */
#include "../drivers/oled_drivers/appstacle_logo.c"

#include <roverapp.h>

void *OLED_Task (void * arg)
{
	timing oled_task_tmr;
	oled_task_tmr.setTaskID("OLED");
	oled_task_tmr.setDeadline(0.5);
	oled_task_tmr.setPeriod(0.5);

	int counter_500ms = 0;

	RoverDisplay my_display = r.inRoverDisplay();

	while (1)
	{
		oled_task_tmr.recordStartTime();
		oled_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		if ( shutdown_hook_shared == 0)
		{
			/* If the display is not in use somewhere else asynchronously */
			if (display_use_elsewhere_shared == 0)
			{

				/* Our internal control-timer actions */
				switch (counter_500ms)
				{
					/* If counter hits counter_500ms * 0.5 sec */
					/* Display APPSTACLE logo in between */
					case 0:
					case 5:
					case 10:
					case 15:
					case 20:
						/* Prepare APPSTACLE logo*/
						my_display.clearDisplay();

						/* Black logo */
						//my_display.fillRect (0, 0, 128, 64, WHITE);
						//my_display.drawBitmap (0, 0, appstacle_logo, 128, 64, BLACK);

						/* White logo */
						my_display.drawBitmap (0, 0, appstacle_logo, 128, 64, WHITE);

						break;

					case 1: /* If counter hits counter_500ms * 0.5 sec */
						/* Prepare WLAN availability */
						my_display.clearDisplay();

						//my_display.drawRect(0, 0, display.width(), display.height(), WHITE);
						//my_display.drawRect(2, 2, display.width()-4, display.height()-4, WHITE);

						my_display.setTextSize(2);
						my_display.setTextColor(WHITE);

						my_display.setCursor(45,10);
						my_display.print("WLAN:");

						my_display.setTextSize(3);
						my_display.setTextColor(WHITE);

						if (r.inRoverUtils().getWlanStatus() == 1)
						{
							my_display.setCursor(50,32);
							my_display.print("ON");
						}
						else
						{
							my_display.setCursor(43,32);
							my_display.print("OFF");
						}

						break;

					case 6: /* If counter hits counter_500ms * 0.5 sec */
						/* Prepare Ethernet availability*/
						my_display.clearDisplay();

						//my_display.drawRect(0, 0, display.width(), display.height(), WHITE);
						//my_display.drawRect(2, 2, display.width()-4, display.height()-4, WHITE);

						my_display.setTextSize(2);
						my_display.setTextColor(WHITE);

						my_display.setCursor(48,10);
						my_display.print("ETH:");

						my_display.setTextSize(3);
						my_display.setTextColor(WHITE);

						if (r.inRoverUtils().getEthernetStatus() == 1)
						{
							my_display.setCursor(50,32);
							my_display.print("ON");
						}
						else
						{
							my_display.setCursor(43,32);
							my_display.print("OFF");
						}

						break;

					case 11: /* If counter hits counter_500ms * 0.5 sec */
						/* Prepare Internet availability */
						my_display.clearDisplay();

						//my_display.drawRect(0, 0, display.width(), display.height(), WHITE);
						//my_display.drawRect(2, 2, display.width()-4, display.height()-4, WHITE);

						my_display.setTextSize(2);
						my_display.setTextColor(WHITE);

						my_display.setCursor(12,10);
						my_display.print("INTERNET:");

						my_display.setTextSize(3);
						my_display.setTextColor(WHITE);

						if (r.inRoverUtils().getInternetStatus() == 1)
						{
							my_display.setCursor(50,32);
							my_display.print("ON");
						}
						else
						{
							my_display.setCursor(43,32);
							my_display.print("OFF");
						}

						break;

					case 16: /* If counter hits counter_500ms * 0.5 sec */
						/* Prepare Bluetooth availability */
						my_display.clearDisplay();

						//my_display.drawRect(0, 0, display.width(), display.height(), WHITE);
						//my_display.drawRect(2, 2, display.width()-4, display.height()-4, WHITE);

						my_display.setTextSize(2);
						my_display.setTextColor(WHITE);

						my_display.setCursor(8,10);
						my_display.print("BLUETOOTH:");

						my_display.setTextSize(3);
						my_display.setTextColor(WHITE);

						if (r.inRoverUtils().getBluetoothStatus() == 1)
						{
							my_display.setCursor(50,32);
							my_display.print("ON");
						}
						else
						{
							my_display.setCursor(43,32);
							my_display.print("OFF");
						}

						break;

					case 21: /* If counter hits counter_500ms * 0.5 sec */
						/* Prepare Hono Instance (Cloud) availability */
						my_display.clearDisplay();

						//my_display.drawRect(0, 0, display.width(), display.height(), WHITE);
						//my_display.drawRect(2, 2, display.width()-4, display.height()-4, WHITE);

						my_display.setTextSize(2);
						my_display.setTextColor(WHITE);

						my_display.setCursor(45,10);
						my_display.print("HONO:");

						my_display.setTextSize(3);
						my_display.setTextColor(WHITE);

						if (r.inRoverUtils().getHonoCloudStatus("idial.institute",8080,"DEFAULT_TENANT", "4711","sensor1","hono-secret") == 1)
						{
							my_display.setCursor(50,32);
							my_display.print("ON");
						}
						else
						{
							my_display.setCursor(43,32);
							my_display.print("OFF");
						}

						break;

					default: /* None of the above */
						// Wait
						break;

				} /* switch-end */

			} /* if display_use_elsewhere_shared */
			else
			{

			}
		}
		else
		{
			// Proper shutdown function, including showing message in the OLED display
			r.shutdown();
		}

		/* If the display is not in use somewhere else asynchronously */
		if (display_use_elsewhere_shared == 0)
		{
			/* Display the stuff NOW */
			my_display.display();

			/* Increment the counter */
			counter_500ms += 1;

			/* Since only first 25*0.5 seconds we do something, we can clear the variable */
			if (counter_500ms > 25)
				counter_500ms = 0;
		}

		//Task content ends here -------------------------------------------------

		oled_task_tmr.recordEndTime();
		oled_task_tmr.calculateExecutionTime();
		oled_task_tmr.calculateDeadlineMissPercentage();
		oled_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&oled_task_ti_l);
			oled_task_ti.deadline = oled_task_tmr.getDeadline();
			oled_task_ti.deadline_miss_percentage = oled_task_tmr.getDeadlineMissPercentage();
			oled_task_ti.execution_time = oled_task_tmr.getExecutionTime();
			oled_task_ti.period = oled_task_tmr.getPeriod();
			oled_task_ti.prev_slack_time = oled_task_tmr.getPrevSlackTime();
			oled_task_ti.task_id = oled_task_tmr.getTaskID();
			oled_task_ti.start_time = oled_task_tmr.getStartTime();
			oled_task_ti.end_time = oled_task_tmr.getEndTime();
		pthread_mutex_unlock(&oled_task_ti_l);
		oled_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}



