/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    CPU Utilization Logger Task for Rover / Raspberry Pi - uses external python script
 *    Header file
 *
 * Note:
 *    read_core_usage.py must be installed to /opt/read_core_usage.py
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 */

#ifndef TASKS_CPU_LOGGER_TASK_H_
#define TASKS_CPU_LOGGER_TASK_H_

#include <stdio.h>
#include <stdlib.h>

/* Interfaces */
void *Cpu_Logger_Task (void * arg);



#endif /* TASKS_CPU_LOGGER_TASK_H_ */
