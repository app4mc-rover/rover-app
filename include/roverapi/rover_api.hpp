/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover API - Interfaces for Rover application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef ROVER_API_HPP_
#define ROVER_API_HPP_

#include <stdint.h>

/**
  *   @brief  rover Namespace contains classes to manage Rover sensors, gpio, driving, utilities, cloud, and web UI.
  *
  */
namespace rover
{
	class RoverBase
	{

		public:
			RoverBase();
			virtual ~RoverBase();
			void initialize (void);
			void start (void);
			void kill (void);
	};
}



#endif /* ROVER_API_HPP_ */
