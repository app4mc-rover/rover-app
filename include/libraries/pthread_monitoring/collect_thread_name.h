/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Function to collect thread name in a file for trace analysis
 *
 * Authors:
 *    M. Ozcelikors,
 *    <mozcelikors@gmail.com>
 *
 * Contributors:
 *
 * Update History:
 *    30.05.2017 - function created
 *
 */

#ifndef PTHREAD_MONITORING_COLLECT_THREAD_NAME_H_
#define PTHREAD_MONITORING_COLLECT_THREAD_NAME_H_


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <cstdio>


using namespace std;

void CollectThreadName(char * thread_name);
void RefreshThreadList(void);
void CollectProcessID(void);


#endif /* PTHREAD_MONITORING_COLLECT_THREAD_NAME_H_ */
