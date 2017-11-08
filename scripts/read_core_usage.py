#!/usr/bin/env python
# Copyright (c) 2017 FH Dortmund.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Description:
#    roverapp | Script that returns core utilization using python's psutil module
#
# Contributors:
#    M. Ozcelikors <mozcelikors@gmail.com>
#
# Installation:
# 	Install/copy this file to /opt/rover-app/scripts

import psutil
import time
import string
import sys
import os

def main():
	try:
		core_utilization = psutil.cpu_percent(interval=2, percpu=True)
		print core_utilization
		sys.exit(os.EX_OK)
	except Exception as inst:
		sys.exit(os.EX_SOFTWARE)
		
if (__name__=="__main__"):
	main()
