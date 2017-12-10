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

#include <libraries/timing/timing.h>

timing::timing() {
	this->_TASK_ID = "None";
	this->_DEADLINE_MISS_PERCENTAGE = 0;
	this->_START_TIME = (clock_t) 0.0;
	this->_END_TIME = (clock_t) 0.0;
	this->_DEADLINE = 0.0;
	this->_PERIOD = 0.0;
	this->_EXECUTION_TIME=0.0;
	this->_TOTAL_CYCLES=0;
	this->_MISSED_CYCLES=0;
	this->_PREV_SLACK_TIME=0.0;
}

timing::~timing() {
	// TODO Auto-generated destructor stub
}

void timing::setDeadline(double deadline)
{
	this->_DEADLINE = deadline;
}

void timing::setPeriod(double period)
{
	this->_PERIOD  = period;
}

void timing::recordStartTime(void)
{
	this->_START_TIME = std::clock();
}

void timing::recordEndTime(void)
{
	this->_END_TIME = std::clock();
}

void timing::calculateExecutionTime(void)
{
	this->_EXECUTION_TIME = ( this->_END_TIME - this->_START_TIME ) / (double) CLOCKS_PER_SEC;
}

void timing::calculatePreviousSlackTime(void)
{
	this->_PREV_SLACK_TIME = ( this->_START_TIME - this->_END_TIME ) / (double) CLOCKS_PER_SEC;
}

void timing::calculateDeadlineMissPercentage(void)
{
	this->_DEADLINE_MISS_PERCENTAGE =  (int)   (this->_MISSED_CYCLES  / (double) this->_TOTAL_CYCLES)*100;
}

int timing::getTotalCycles(void)
{
	return this->_TOTAL_CYCLES;
}
int timing::getMissedCycles(void)
{
	return this->_MISSED_CYCLES;
}
double timing::getDeadline(void)
{
	return this->_DEADLINE;
}
double timing::getPeriod(void)
{
	return this->_PERIOD;
}
double timing::getStartTime(void)
{
	return this->_START_TIME / (double) CLOCKS_PER_SEC;
}
double timing::getEndTime(void)
{
	return this->_END_TIME / (double) CLOCKS_PER_SEC;
}
double timing::getPrevSlackTime(void)
{
	return this->_PREV_SLACK_TIME;
}
double timing::getExecutionTime(void)
{
	return this->_EXECUTION_TIME;
}
double timing::getDeadlineMissPercentage(void)
{
	return this->_DEADLINE_MISS_PERCENTAGE;
}
void timing::setTaskID(char * taskid)
{
	this->_TASK_ID = taskid;
}
char * timing::getTaskID(void)
{
	return this->_TASK_ID;
}
void timing::incrementTotalCycles(void)
{
	this->_TOTAL_CYCLES = this->_TOTAL_CYCLES + 1;
}
void timing::incrementMissedCycles(void)
{
	this->_MISSED_CYCLES = this->_MISSED_CYCLES + 1;
}
void timing::sleepToMatchPeriod(void)
{
	if (this->_EXECUTION_TIME > this->_DEADLINE)
	{
		this->_MISSED_CYCLES = this -> _MISSED_CYCLES + 1;
	}
	else
	{
		#if SIMULATOR
			usleep(getPeriod() * SECONDS_TO_MICROSECONDS);
		#else
			delayMicroseconds(getPeriod() * SECONDS_TO_MICROSECONDS);
		#endif
	}
}
