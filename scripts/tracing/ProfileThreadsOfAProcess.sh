#!/bin/bash
# Copyright (c) 2017 FH Dortmund and others
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Description:
#	This script is used for estimating the granularity of a thread by
#	making use of its name and period.
#	Example usage: ./ProfileThreadsOfAProcess.sh -n <process_name> -t <period> -c <path/to/perf/obj>
#					<period> is in milliseconds
#	
# Authors:
#	M. Ozcelikors <mozcelikors@gmail.com>, FH Dortmund
#

helpwindow ()
{
	echo "Arguments for ProfileThreadsOfAProcess.sh are:"
	echo "#################################################"
	echo "-h or --help : Show these help instructions"
	echo "-p or --pid  : Progress using Process ID"
	echo "-n or --name : Progress using Process Name (CMD)"
	echo "-t or --time : Specify period in milliseconds to analyze"
	echo "-c or --command : Perf command or path to perf object to run"
	echo "#################################################"
	echo "Example usage: sudo ./ProfileThreadsOfAProcess.sh -n <process_name> -t <period> -c <path/to/perf/obj>"
	echo "or sudo ./ProfileThreadsOfAProcess.sh -p <pid> -t <period> -c <path/to/perf/obj>"
}

if [ "$1" == "" ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	helpwindow
	exit 0
fi

while [[ $# -gt 1 ]]
do
	key="$1"
	case $key in
		-p|--pid)
			pid="$2"
			shift
		;;
		-n|--name)
			process_name="$2"
			shift
		;;
		-t|--time)
			period="$2"
			shift
		;;
		-c|--command)
			perf_command="$2"
			shift
		;;
		*)
			#Unknown
			helpwindow
		;;
	esac
	shift
done

if ! [[ -z ${perf_command+x} ]]; then
	if ! [[ -z ${period+x} ]]; then
		if ! [[ -z ${pid+x} ]]; then
			$perf_command stat --per-thread -e instructions:u -p $pid -I $period 
		elif ! [[ -z ${process_name+x} ]]; then
			pid=$(pgrep -f $process_name -o)
			$perf_command stat --per-thread -e instructions:u -p $pid -I $period 
		fi
	else
		helpwindow
	fi
else
	helpwindow
fi

exit $?


