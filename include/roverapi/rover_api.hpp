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

#include <roverapi/rover_display.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_gpio.hpp>
#include <roverapi/rover_sensors.hpp>
#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_cloud.hpp>

/**
\mainpage Rover API Main Page

\section api_info roverapp and Rover API Info

Rover software, called **roverapp** features a multi-threaded (POSIX threads or Pthreads) C/C++ implementation that runs on Linux-based embedded single board computers (such as Raspberry Pi).
Rover features countless threads dedicated to communication infrastructure, sensor driving, display unit (such as OLED displays) utilization, bluetooth communication, image processing, and behavior modes (such as Parking, Adaptive Cruise Control, Manual Driving, and Booth Modes).
It also features drivers for sensors such as magnetometers, accelerometers, various ultrasonic sensors, and camera modules. Furthermore, OLED display, buttons, a buzzer are utilized.

Roverapp builds and contains the **Rover API**, which is able to handle subset of its functionality. Example functionality covered in **Rover API** is given below:

\li **RoverBase** class is the main class used for instantiating rover objects.
Each RoverBase object has private member functions referencing to other API classes exclusive to one RoverBase object.
Therefore, using one RoverBase class, other classes can be accessed, and operations such as initialization,
shutting down could be performed.
\li **RoverCloud** contains the member functions to connect and send data to Eclipse Hono instance using several parameters such as host name, port, tenant name, user name, and password. This class wraps hono_interaction library for Rover-specific interactions.
\li **RoverDisplay** contains the member functions to control OLED display on the Rover. This class is a wrapper API for Adafruit_GFX and Adafruit_SSD1306 libraries.
\li **RoverDriving** contains the member functions to drive the rover using its motors.
\li **RoverGpio** class provides the member functions related to embedded buzzer, user button, and shutdown button on the rover.
\li **RoverSensors** class contains member functions to setup and read from embedded sensors from rover's RoverSenseLayer sensor layer.
\li **RoverUtils** contains member functions to retrieve connectivity status and core utilization of the rover. This class wraps status_library lib, developed for the rover-app.

\image html ./images/rover2.jpg

\section example_usage Rover API Example Usage
The following is an example C++ application using some of the Rover API functions:

\code{.cpp}
#include <roverapi/rover_api.hpp>

using namespace rover;

int main (void)
{
	RoverBase r;

	// Initialize all components of the rover
	r.initialize();
    // or use r.initializeWiringPi(), r.initializeRoverSensors(), r.initializeRoverDisplay(), r.initializeRoverDriving(), r.initializeRoverGpio() to initialize individual components

	// Set-up cloud instance and register your device
	RoverCloud r_cloud = r.inRoverCloud();

	r_cloud.setHono("localhost", 8080, "DEFAULT_TENANT");

	r_cloud.setRegistrationPort(28080);

	if (r_cloud.registerDevice("4711") == 1)
	{
		printf ("Registered device to Hono cloud using REST API successfully..\n");
	}

	// Send telemetry data to Hono instance
	if (r_cloud.sendTelemetry("4711","myuser","mypassword","roverFront", 100.0) == 1)
	{
		printf ("Data sent to Hono cloud using REST API successfully..\n");
	}

	// Driving with rover
	r.inRoverDriving().setSpeed(r.inRoverDriving().HIGHEST_SPEED);
	r.inRoverDriving().goForward();
	r.sleep (500); // Sleep for some time in milliseconds
	r.inRoverDriving().turnRight();
	r.sleep (500); // Sleep for some time in milliseconds
	r.inRoverDriving().stopRover();


	// Accessing sensors with rover
	RoverSensors r_sensors = r.inRoverSensors();

	printf ("Ultrasonic = [%d %d]\n",	r_sensors.readHCSR04UltrasonicSensor(r_sensors.ROVER_FRONT),
										r_sensors.readHCSR04UltrasonicSensor(r_sensors.ROVER_REAR));
	printf ("Infrared = [%f %f %f %f]\n", 	r_sensors.readInfraredSensor(r_sensors.ROVER_FRONT_LEFT),
											r_sensors.readInfraredSensor(r_sensors.ROVER_FRONT_RIGHT),
											r_sensors.readInfraredSensor(r_sensors.ROVER_REAR_LEFT),
											r_sensors.readInfraredSensor(r_sensors.ROVER_REAR_RIGHT));
	printf ("Temperature = %f\n",	r_sensors.readTemperature());
	printf ("Humidity = %f\n",		r_sensors.readHumidity());
	printf ("Bearing = %f\n",		r_sensors.readBearingHMC5883L());

	// Checking if a button is pressed (LOW) and playing a tone with buzzer
	if (r.inRoverGpio().readUserButton() == r.inRoverGpio().LO)
	{
		r.inRoverGpio().setBuzzerFrequency(400); //in Hz
		r.inRoverGpio().setBuzzerOn();
		r.sleep(1000);
	}
	r.inRoverGpio.setBuzzerOff();

	// Print core utilization from the rover's OS
	float *util = r.inRoverUtils.getCoreUtilization();
	printf ("Utilization = [%f %f %f %f]\n", util);

	// Use the OLED display on the rover
	RoverDisplay my_display = r.inRoverDisplay();

	// Prepare display contents
	my_display.clearDisplay();
	my_display.setTextSize(2);
	my_display.setTextColor(my_display.WHITE_COLOR);

	my_display.setCursor(12,10);
	my_display.print("INTERNET:");

	my_display.setTextSize(3);
	my_display.setTextColor(my_display.WHITE_COLOR);

	// Check if internet is connected
	if (r.inRoverUtils().getInternetStatus() == 1)
	{
		my_display.setCursor(50,32);
		my_display.print("ON");
	}
	else
	{
		my_display.setCursor(43,32);
		my_display.print("OFF");
	}

	// Display now
	my_display.display();

	// Sleep a bit
	r.sleep(5000);

	// Shutdown the rover OS and abort the application
	r.shutdown();

	return 1;
}
\endcode

\section roverdocs Rover Documentation

Link: <a href="http://app4mc-rover.github.io/rover-docs">Rover Complete Documentation</a>

\copyright Copyright(C) 2017 IDiAL Institute, Fachhochschule Dortmund

*/

/**
  *   @brief  rover Namespace contains classes to manage Rover sensors, gpio, driving, utilities, and cloud.
  */
namespace rover
{
	/**
	  *   @brief  RoverBase class is the main class used for instantiating rover objects.
	  *   Each RoverBase object has private member functions referencing to other API classes exclusive to one RoverBase object.
	  *   Therefore, using one RoverBase class, other classes can be accessed, and operations such as initialization,
	  *   shutting down could be performed.
	  */
	class RoverBase
	{
		private:
			/**
			 * @brief Private RoverCloud instance that is accessed from RoverBase
			 */
			RoverCloud myRoverCloud;

			/**
			 * @brief Private RoverDisplay instance that is accessed from RoverBase
			 */
			RoverDisplay myRoverDisplay;

			/**
			 * @brief Private RoverDriving instance that is accessed from RoverBase
			 */
			RoverDriving myRoverDriving;

			/**
			 * @brief Private RoverGpio instance that is accessed from RoverBase
			 */
			RoverGpio myRoverGpio;

			/**
			 * @brief Private RoverSensors instance that is accessed from RoverBase
			 */
			RoverSensors myRoverSensors;

			/**
			 * @brief Private RoverUtils instance that is accessed from RoverBase
			 */
			RoverUtils myRoverUtils;

			/**
			 * @brief Flag for indicating whether private RoverGpio object initialized or not.
			 */
			int ROVER_GPIO_INIT_;

			/**
			 * @brief Flag for indicating whether private RoverSensors object initialized or not.
			 */
			int ROVER_SENSORS_INIT_;

			/**
			 * @brief Flag for indicating whether private RoverDisplay object initialized or not.
			 */
			int ROVER_DISPLAY_INIT_;

			/**
			 * @brief Flag for indicating whether private RoverDriving object initialized or not.
			 */
			int ROVER_DRIVING_INIT_;

			/**
			 * @brief Flag for indicating whether wiringPi library initialized or not.
			 */
			int WIRINGPI_INIT_;


		public:
			/**
			 * @brief Constructor for the RoverBase class
			 */
			explicit RoverBase();

			/**
			 * @brief Destructor for the RoverBase class
			 */
			virtual ~RoverBase();

			/**
			  *   @brief  Initializes the all classes, sensors, libraries for the Rover.
			  *   @return void
			  *
			  *   \warning This function can only be used once in Rover's Raspberry Pi. You can still use a second RoverBase object and configure that object. However, initializing in the low-level sense causes problems.
			  *   \code{.cpp}
			  *   RoverBase r;
			  *   RoverBase x;
			  *   r.initialize();
			  *   x.initialize(); //^^^THIS IS NOT VALID AND WILL PRODUCE ERRORS
			  *   \endcode
			  */
			void initialize (void);

			/**
			 * @brief Shuts down the Rover's OS.
			 * @return void
			 */
			void shutdown (void);

			/**
			 * @brief Sleep function to be used in rover applications
			 * @param period_ms Period to sleep in milliseconds
			 * @return void
			 */
			void sleep (const unsigned int period_ms);

			/**
			 * @brief Public function to access private RoverCloud instance.
			 * @return RoverCloud instance
			 */
			rover::RoverCloud& inRoverCloud (void);

			/**
			 * @brief Public function to access private RoverDisplay instance.
			 * @return RoverDisplay instance
			 */
			rover::RoverDisplay& inRoverDisplay (void);

			/**
			 * @brief Public function to access private RoverDriving instance.
			 * @return RoverDriving instance
			 */
			rover::RoverDriving& inRoverDriving (void);

			/**
			 * @brief Public function to access private RoverGpio instance.
			 * @return RoverGpio instance
			 */
			rover::RoverGpio& inRoverGpio (void);

			/**
			 * @brief Public function to access private RoverSensors instance.
			 * @return RoverSensors instance
			 */
			rover::RoverSensors& inRoverSensors (void);

			/**
			 * @brief Public function to access private RoverUtils instance.
			 * @return RoverUtils instance
			 */
			rover::RoverUtils& inRoverUtils (void);

			/**
			 * @brief Initializes private RoverDriving object instance.
			 * @return void
			 */
			void initializeRoverDriving (void);

			/**
			 * @brief Initializes private RoverGpio object instance.
			 * @return void
			 */
			void initializeRoverGpio (void);

			/**
			 * @brief Initializes private RoverDisplay object instance.
			 * @return void
			 */
			void initializeRoverDisplay (void);

			/**
			 * @brief Initializes private RoverSensors object instance.
			 * @return void
			 */
			void initializeRoverSensors (void);

			/**
			  *   @brief  Initializes wiringPi library to access GPIO of Rover. This function should be called in every program run and must only be called once.
			  *   @return void
			  */
			void initializeWiringPi (void);
	};
}



#endif /* ROVER_API_HPP_ */
