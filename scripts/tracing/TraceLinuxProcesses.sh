#!/bin/bash
# Copyright (c) 2017 FH Dortmund and others
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Description:
#	Linux shell (bash) script that traces linux processes and converts
#	the trace to common tracing format (CTF).
#	perf module should be compiles with libbabeltrace in order to run
#	the tracing utility correctly.
#	Created out directory will contain Processes_List.txt (process IDs
#	are listed), perf.data (trace in perf format), ctf_trace.tar.gz
#	(trace in ctf format)
#	The ctf_format.tar.gz can be extracted in order to visualize the 
#	perf streams using Eclipse TraceCompass.
#
# Usage:
#	sudo ./TraceLinuxProcesses.sh -p <path/to/perf> -n <trace_name> -t <period_to_trace>
#
# Authors:
#	M. Ozcelikors <mozcelikors@gmail.com>, FH Dortmund
#

helpwindow ()
{
	echo "Arguments for TraceLinuxProcesses.sh are:"
	echo "#################################################"
	echo "-h or --help : 			Show these help instructions"
	echo "-p or --path-to-perf : 	Path to Perf"
	echo "-n or --name : 			Trace name to be created"
	echo "-t or --time : 			Specify period in seconds to trace"
	echo "#################################################"
	echo "Example usage: sudo ./TraceLinuxProcesses.sh -p <path/to/perf> -n <trace_name> -t <period_to_trace>"
}

if [ "$1" == "" ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	helpwindow
	exit 0
fi

while [[ $# -gt 1 ]]
do
	key="$1"
	case $key in
		-p|--path-to-perf)
			perf_directory="$2"
			shift
		;;
		-n|--name)
			trace_name="$2"
			shift
		;;
		-t|--time)
			seconds="$2"
			shift
		;;
		*)
			#Unknown
			helpwindow
		;;
	esac
	shift
done

if ! [[ -z ${perf_directory+x} ]]; then
	if ! [[ -z ${trace_name+x} ]]; then
		if ! [[ -z ${seconds+x} ]]; then
			echo "### Creating directory.."
			sudo mkdir out_$trace_name/
			echo "### Writing out process names.."
			ps -aux >> out_$trace_name/Processes_List.txt
			echo "### Tracing with perf for $seconds seconds.."
			sudo $perf_directory/./perf sched record -o out_$trace_name/perf.data -- sleep $seconds
			echo "### Converting to data to CTF (Common Tracing Format).."
			sudo LD_LIBRARY_PATH=/opt/libbabeltrace/lib $perf_directory/./perf data convert -i out_$trace_name/perf.data --to-ctf=./ctf
			sudo tar -czvf out_$trace_name/trace.tar.gz ctf/
			sudo rm -rf ctf/
			
			echo "### Process IDs are written to out_$trace_name/Processes_List.txt"
			echo "### Trace in Perf format is written to out_$trace_name/perf.data"
			echo "### Trace in CTF format is written to out_$trace_name/ctf_trace.tar.gz"
			echo "### Exiting.."
		else
			helpwindow
		fi
	else
		helpwindow
	fi
else
	helpwindow
fi

exit $?




