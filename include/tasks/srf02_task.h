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

#ifndef TASKS_SRF02_TASK_H_
#define TASKS_SRF02_TASK_H_

/* Defines */
#define FRONT_SRF02_ADDR ((int) 0x70)
#define REAR_SRF02_ADDR  ((int) 0x73)

/* Interfaces */
void *SRF02_Task(void * arg);


#endif /* TASKS_SRF02_TASK_H_ */
