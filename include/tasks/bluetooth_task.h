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
 *    For Eclipse APP4MC Rover - header file
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


#ifndef TASKS_BLUETOOTH_TASK_H_
#define TASKS_BLUETOOTH_TASK_H_


/* Intefaces */
void *Bluetooth_Task(void * arg);


#endif /* TASKS_BLUETOOTH_TASK_H_ */
