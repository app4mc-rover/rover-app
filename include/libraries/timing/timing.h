/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Timing class created for thread performance analysis.
 *
 * Authors:
 *    M. Ozcelikors,
 *    <mozcelikors@gmail.com>
 *
 * Contributors:
 *
 * Update History:
 *    23.03.2017 - initial revision
 *
 */

#ifndef TIMING_TIMING_H_
#define TIMING_TIMING_H_

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <wiringPi.h>

#define SECONDS_TO_MICROSECONDS 1000000
#define MICROSECONDS_TO_SECONDS 0.000001

class timing {
public:
	timing();
	virtual ~timing();

	void recordStartTime();
	void recordEndTime();
	void calculateExecutionTime();
	void calculatePreviousSlackTime();
	void calculateDeadlineMissPercentage();
	void sleepToMatchPeriod(void);

	void setDeadline(double deadline);
	void setPeriod(double period);
	void setTaskID(char * taskid);

	void incrementTotalCycles(void);
	void incrementMissedCycles(void);

	int getTotalCycles(void);
	int getMissedCycles(void);
	double getDeadline(void);
	double getPeriod(void);
	double getPrevSlackTime(void);
	double getExecutionTime(void);
	double getDeadlineMissPercentage(void);
	char * getTaskID(void);

	double getStartTime(void);
	double getEndTime(void);

private:
	char * _TASK_ID;
	int _TOTAL_CYCLES;
	int _MISSED_CYCLES; //Deadline misses
	double _DEADLINE;
	double _PREV_SLACK_TIME;
	double _PERIOD;
	double _EXECUTION_TIME;
	int _DEADLINE_MISS_PERCENTAGE;
	std::clock_t _START_TIME;
	std::clock_t _END_TIME;
};

#endif /* TIMING_TIMING_H_ */
