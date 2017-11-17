/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Cloud API - Interfaces for Rover cloud application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_CLOUD_HPP_
#define API_ROVER_CLOUD_HPP_

#include <api/rover_api.hpp>

namespace rover
{
	/**
	 * @brief Contains the member functions to connect and send data to Eclipse Hono instance using several parameters
	 * such as host name, port, tenant name, user name, and password. This class wraps hono_interaction library for Rover-specific interactions.
	 */
	class RoverCloud : public RoverBase
	{
		private:
			/**
			 * @brief Host name used for connecting to the Eclipse Hono using REST API
			 */
			char * HOST_NAME;
			/**
			 * @brief Port used for connecting to the Eclipse Hono using REST API
			 */
			int PORT;
			/**
			 * @brief Tenant name used for connecting to the Eclipse Hono using REST API
			 */
			char * TENANT_NAME;
		public:
			/**
			 * @brief Registers a device to Hono instance given device ID
			 * @return status message is returned. 1 indicates successful operation and 0 indicates unsuccessful operation.
			 */
			void setHostName (char * host_name);

			/**
			 * @brief Use brief, otherwise the index won't have a brief explanation.
			 * @param  a is an initialized integer variable
			 * @param  b is an initialized integer variable
			 * @return void
			 *
			 * Detailed explanation.
			 */
			void setPort (int port);
			void setTenantName (char * tenant);
			void setHono (char * host_name, int port, char * tenant);
			char * getHostName (void);
			int getPort (void);
			char * getTenantName (void);
			int registerDevice (char * device_id);
			int sendTelemetry (char * device_id, char * user, char * password, char * field, double value);
			int sendEvent (char * device_id, char * user, char * password, char * field, double value);
			int getHonoStatus (char * device_id, char * user, char * password);
	};
}


#endif /* API_ROVER_CLOUD_HPP_ */
