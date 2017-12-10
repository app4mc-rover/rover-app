/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Utils API - Interfaces for Rover utilities application development - Header file
 *
 * Usage Note:
 *    read_core_usage.py must be installed to /opt/rover-app/scripts/read_core_usage.py
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_UTILS_HPP_
#define API_ROVER_UTILS_HPP_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

namespace rover
{
	/**
	 * @brief RoverUtils contains member functions to retrieve connectivity status and core utilization of the rover. This class wraps status_library lib, developed for the rover-app.
	 */
	class RoverUtils
	{

		public:
			/** Number of cores available in the rover's Raspberry Pi */
			static const int numberOfCores = 4;

			/**
			 * @brief Obtain the core utilization of each core in rover's Raspberry Pi.
			 * Uses an external script called read_core_usage.py which must be installed within rover-app.
			 * @param coreUtil is a float array that contains core usage of each core in rover's Raspberry Pi.
			 */
			void getCoreUtilization (float coreUtil[numberOfCores]);

			/**
			 * @brief Returns whether the wlan0 interface is on (1) or off (0).
			 * @return status wlan0 interface status : on (1) or off (0)
			 */
			int getWlanStatus (void);

			/**
			 * @brief Returns whether the eth0 interface is on (1) or off (0).
			 * @return status eth0 interface status : on (1) or off (0)
			 */
			int getEthernetStatus (void);

			/**
			 * @brief Returns whether the internet is on (1) or off (0).
			 * @return status internet status : on (1) or off (0)
			 */
			int getInternetStatus (void);

			/**
			 * @brief Returns whether the bluetooth is on (1) or off (0).
			 * @return status bluetooth status : on (1) or off (0)
			 */
			int getBluetoothStatus (void);

			/**
			 * @brief Returns whether the remote Eclipse Hono instance is on (1) or off (0).
			 * @return status remote Eclipse Hono instance status : on (1) or off (0)
			 */
			int getHonoCloudStatus (char * host_name, int port, char * tenant_name, char * device_id, char * user, char * password);
	};
}


#endif /* API_ROVER_UTILS_HPP_ */
