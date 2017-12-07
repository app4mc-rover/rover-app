/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Cloud API - Virtual class for Rover Cloud application interfacing
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, revised using MQTT 07.12.2017
 *    										created C++API 17.11.2017
 *
 */

#ifndef ROVERAPI_ROVER_CLOUD_HPP_
#define ROVERAPI_ROVER_CLOUD_HPP_

namespace rover
{
	class RoverCloud
	{
		private:
			/**
			 * @brief Host name used for connecting to a cloud server
			 */
			char * HOST_NAME;

			/**
			 * @brief Port used for connecting to a cloud server
			 */
			int PORT;

		public:
			/**
			 * @brief Sets private attribute HOST_NAME
			 * @param host_name
			 */
			virtual void setHostName (char * host_name) = 0;

			/**
			 * @brief Sets private attribute PORT
			 * @param port
			 */
			virtual void setPort (const int port) = 0;

			/**
			 * @brief Virtual destructor
			 */
			virtual ~RoverCloud() {};

	};
}



#endif /* ROVERAPI_ROVER_CLOUD_HPP_ */
