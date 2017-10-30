/*******************************************************************************
 * Copyright (c) 2017 FH-Dortmund and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     FH-Dortmund - initial API and implementation
 *******************************************************************************/

#ifndef TASKS_ADAPTIVE_CRUISE_CONTROL_TASK_H_
#define TASKS_ADAPTIVE_CRUISE_CONTROL_TASK_H_


#include <stdio.h>
#include <stdlib.h>


#define CRITICAL_DISTANCE 10
#define CORRECT_DISTANCE 20
#define SAFE_DISTANCE 40

void *Adaptive_Cruise_Control_Task(void * arg);


#endif /* TASKS_ADAPTIVE_CRUISE_CONTROL_TASK_H_ */
