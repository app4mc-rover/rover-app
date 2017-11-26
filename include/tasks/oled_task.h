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

#ifndef TASKS_OLED_TASK_H_
#define TASKS_OLED_TASK_H_

/* Defines */

/* Intefaces */
void *OLED_Task(void * arg);

#endif /* TASKS_OLED_TASK_H_ */
