#!/bin/bash
# Copyright (c) 2017 FH Dortmund and others
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Description:
#	This script is used for monitoring core utilization of all threads of
#	a process by the process name.
#	Right usage: 	./MonitorThreads.sh -n <process_name>
#					./MonitorThreads.sh -p <pid>
#	You can access the help instructions by typing ./MonitorThreads.sh --help
#
# Authors:
#	M. Ozcelikors <mozcelikors@gmail.com>, FH Dortmund
#

helpwindow ()
{
	echo "Arguments for MonitorThreads.sh are:"
	echo "#################################################"
	echo "-h or --help : Show these help instructions"
	echo "-p or --pid  : Progress using Process ID"
	echo "-n or --name : Progress using Process Name (CMD)"
	echo "#################################################"
	echo "Example usage: sudo ./MonitorThreads -n MyApplication"
	echo "               sudo ./MonitorThreads -p 1550"
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
		*)
			#Unknown
			helpwindow
		;;
	esac
	shift
done


if ! [[ -z ${pid+x} ]]; then
	ps H -p $pid -o 'pid tid cmd comm'
	top H -p $pid
elif ! [[ -z ${process_name+x} ]]; then
	pid=$(pgrep -f $process_name -o)
	ps H -p $pid -o 'pid tid cmd comm'
	top H -p $pid
fi

exit $?
