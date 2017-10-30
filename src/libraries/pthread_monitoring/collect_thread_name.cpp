/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Function to collect thread name, process id, and kernel thread ids in a file for trace analysis
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

#include <libraries/pthread_monitoring/collect_thread_name.h>

#include <syscall.h>
#include <sys/types.h>
#include <unistd.h>

pid_t gettid(void)
{
    return(syscall(SYS_gettid));
}

void CollectProcessID(void)
{
	ofstream myfile;

	uint64_t process_id = getpid();

	myfile.open("/home/pi/Threads_List.txt", std::ios::app); //Append

	myfile << "Process ID = " <<  process_id << "\n";
	myfile.close();
}

void CollectThreadName(char * thread_name)
{
	ofstream myfile;

	uint64_t thread_id = gettid();

	myfile.open("/home/pi/Threads_List.txt", std::ios::app); //Append

	myfile << thread_name << " " << thread_id << "\n";
	myfile.close();

}

void RefreshThreadList (void)
{
	ifstream fin("/home/pi/Threads_List.txt");
	if (fin)
	{
		fin.close();
		system("rm -rf /home/pi/Threads_List.txt");
	}
}
