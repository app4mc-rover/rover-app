/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover API - Interfaces for basic Rover application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef ROVER_API_HPP_
#define ROVER_API_HPP_

#include <stdint.h>


/**
\mainpage Rover API Main Page

\section api_info roverapp and Rover API Info

Rover software, called **roverapp** features a multi-threaded (POSIX threads or Pthreads) C/C++ implementation that runs on Linux-based embedded single board computers (such as Raspberry Pi).
Rover features countless threads dedicated to communication infrastructure, sensor driving, display unit (such as OLED displays) utilization, bluetooth communication, image processing, and behavior modes (such as Parking, Adaptive Cruise Control, Manual Driving, and Booth Modes).
It also features drivers for sensors such as magnetometers, accelerometers, various ultrasonic sensors, and camera modules. Furthermore, OLED display, buttons, a buzzer are utilized.

Roverapp builds and contains the **Rover API**, which is able to handle subset of its functionality. Example functionality covered in **Rover API** is given below:

\li **RoverBase** RoverBase class provides basic rover functions such as initialization, sleeping, and shutting down.
\li **RoverDisplay** contains the member functions to control OLED display on the Rover. This class is a wrapper API for Adafruit_GFX and Adafruit_SSD1306 libraries.
\li **RoverDriving** contains the member functions to drive the rover using its motors.
\li **RoverGpio** class provides the member functions related to low-level pin driving. This class wraps wiringPi library and is inherited by RoverButton and RoverBuzzer classes.
\li **RoverBuzzer** class provides the member functions related to embedded buzzer on the rover.
\li **RoverButton** class contains member functions to access Shutdown button, User button, and customly installed buttons on the rover.
\li **RoverUtils** contains member functions to retrieve connectivity status and core utilization of the rover. This class wraps status_library lib, developed for the rover-app.
\li **RoverSensors** is a pure abstract class defining the interface between sensor classes of the rover.

\li **RoverDHT22** is a class that is inherited from RoverSensor abstract class. RoverDHT22 class contains member functions and variables to set up and read from DHT22 temperature and humidity sensor that is embedded on the rover.
\li **RoverGrooveUltrasonic** is a class that is inherited from RoverSensor abstract class. RoverGrooveUltrasonic class contains member functions and variables to set up and read from Groove ultrasonic sensors that are embedded on the rover.
\li **RoverHCSR04** is a class that is inherited from RoverSensor abstract class. RoverHCSR04 class contains member functions and variables to set up and read from HCSR-04 ultrasonic sensors that are embedded on the rover.
\li **RoverInfraredSensor** is a class that is inherited from RoverSensor abstract class. RoverInfraredSensor class contains member functions and variables to set up and read from SHARP infrared sensors that are embedded on the rover.
\li **RoverGY521** is a class that is inherited from RoverSensor abstract class. RoverGY521 class contains member functions and variables to set up and read from GY521 accelerometer that is embedded on the rover.
\li **RoverHMC5883L** is a class that is inherited from RoverSensor abstract class. RoverHMC5883L class contains member functions and variables to set up and read from HMC5883L bearing sensor that is embedded on the rover.

\li **RoverCloud** class is a purely abstract interface for cloud communication classes.
\li **RoverQMC5883L** is a class that is inherited from RoverSensor abstract class. RoverQMC5883L class contains member functions and variables to set up and read from QMC5883L bearing sensor that is embedded on the rover.
\li **RoverHonoCloud** contains the member functions to connect and send data to Eclipse Hono instance using several parameters such as host name, port, tenant name, user name, and password. This class wraps hono_interaction library for Rover-specific interactions.
\li **RoverPahoMQTT** contains member functions to use rover as a client and to publish / subscribe to Eclipse Paho MQTT server topics.
\li **RoverMQTTCommand** class is an implementation class -privately- extending RoverPahoMQTT for rover-specific topic subscription and publishing using JSON for parsing and using predefined type variables such as RoverControlData_t and RoverSensorData_t.

\image html ./images/rover2.jpg


\section overview1 UML Diagram Overview

\image html ./images/RoverAPI_Overview.jpg
Updated: 11.12.2017

\section example_usage Rover API Example Usage
The following is an example C++ application using some of the Rover API functions:

\warning For more concreate examples please go to: https://github.com/app4mc-rover/rover-app/tree/master/src/examples

\code{.cpp}
//Basis Include
#include <roverapi/rover_api.hpp>
//Other Includes
#include <roverapi/rover_honocloud.hpp>
#include <roverapi/rover_dht22.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_button.hpp>
#include <roverapi/rover_grooveultrasonic.hpp>
#include <roverapi/rover_gy521.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_hmc5883l.hpp>
#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/rover_qmc5883l.hpp>
#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_pahomqtt.hpp>
#include <roverapi/rover_mqttcommand.hpp>

using namespace rover;

int main (void)
{
	//This initialization is a one time only must-call before every rover application, initializes low-level GPIO driver
	RoverBase r_base = RoverBase();
	r_base.initialize();

	// Set-up cloud instance and register your device
	RoverHonoCloud r_cloud = RoverHonoCloud ("myhost", 8080, 28080, "DEFAULT_TENANT");

	// To override the initial RoverCloud set-up
	r_cloud.setHono("localhost", 8080, "DEFAULT_TENANT");
	r_cloud.setRegistrationPort(28080);

	// Register a Device
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
	RoverDriving r_driving = RoverDriving();
	r_driving.initialize();

	r_driving.setSpeed(HIGHEST_SPEED);
	r_driving.goForward();
	r_base.sleep (500); // Sleep for some time in milliseconds
	r_driving.turnRight();
	r_base.sleep (500); // Sleep for some time in milliseconds
	r_driving.stopRover();

	// Instantiation of Rover sensors
	RoverHCSR04 r_front = RoverHCSR04(ROVER_FRONT);
	RoverHCSR04 r_rear = RoverHCSR04(ROVER_REAR);
	// or  RoverGrooveUltrasonic r_rear = RoverGrooveUltrasonic(ROVER_REAR);
	RoverInfraredSensor r_infrared0 = RoverInfraredSensor(ROVER_REAR_RIGHT);
	RoverInfraredSensor r_infrared1 = RoverInfraredSensor(ROVER_REAR_LEFT);
	RoverInfraredSensor r_infrared2 = RoverInfraredSensor(ROVER_FRONT_RIGHT);
	RoverInfraredSensor r_infrared3 = RoverInfraredSensor(ROVER_FRONT_LEFT);
	RoverDHT22 r_dht22 = RoverDHT22();
	RoverHMC5883L r_hmc = RoverHMC5883L();
	// or RoverQMC5883L r_qmc = RoverQMC5883L();

	// Set up ultrasonic sensors
	r_front.initialize();
	r_rear.initialize();

	// Set up infrared sensors
	r_infrared0.initialize();
	r_infrared1.initialize();
	r_infrared2.initialize();
	r_infrared3.initialize();

	// Set up DHT22 temperature and humidity sensor
	r_dht22.initialize();

	// Set up HMC5883L or QMC5883L compass sensor
	r_hmc.initialize();
	//r_qmc.initialize();

	printf ("Ultrasonic = [%f %f]\n",	r_front.read(),
										r_rear.read());
	printf ("Infrared = [%f %f %f %f]\n", 	r_infrared0.read(),
											r_infrared1.read(),
											r_infrared2.read(),
											r_infrared3.read());
	printf ("Bearing with HMC5883L = %f\n",		r_hmc.read());
	// or printf ("Bearing with QMC5883L = %f\n",		r_qmc.read());

	// Instantiate objects to access buzzer and buttons on the rover
	RoverButton user_b = RoverButton (USER_BUTTON);
	RoverButton shutdown_b = RoverButton (SHUTDOWN_BUTTON);
	RoverBuzzer buzzer = RoverBuzzer();

	user_b.initialize();
	shutdown_b.initialize();
	buzzer.initialize();

	// Checking if a button is pressed (LOW) and playing a tone with buzzer
	if (user_b.readButton() == user_b.LO)
	{
		buzzer.setBuzzerFrequency(400); //in Hz
		buzzer.setBuzzerOn();
		r_base.sleep(1000);
	}
	buzzer.setBuzzerOff();

	// Instantiate a RoverUtils object to access member functions that deals with status and performance tasks
	RoverUtils r_utils = RoverUtils(); //RoverUtils does not need to be initialized

	// Print core utilization from the rover's OS
	float util[4];
	r_utils.getCoreUtilization(util);
	printf ("Utilization = [%f %f %f %f]\n", util[0], util[1], util[2], util[3]);

	// Use the OLED display on the rover
	RoverDisplay my_display = RoverDisplay();

	// Prepare display contents
	my_display.clearDisplay();
	my_display.setTextSize(2);
	my_display.setTextColor(my_display.WHITE_COLOR);

	my_display.setCursor(12,10);
	my_display.print("INTERNET:");

	my_display.setTextSize(3);
	my_display.setTextColor(my_display.WHITE_COLOR);

	// Check if internet is connected
	if (r_utils.getInternetStatus() == 1)
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

	// Publish to a topic in an MQTTv3 or MQTTv3.1 broker using RoverPahoMQTT class

	RoverMQTT_Configure_t rover_mqtt_conf;
	rover_mqtt_conf.clientID = "rover";							// Identification of the Client
	rover_mqtt_conf.qos      = 1;   							// Quality of Service
	rover_mqtt_conf.timeout  = 10000L;  						// Polling timeout, 10000L is fine
	rover_mqtt_conf.topic    = "rover/RoverDriving/control/1"; 	// Topic name to publish to or subscribe from
    rover_mqtt_conf.username    = "sensor1@DEFAULT_TENANT";		// Username - Leave empty for no user and password
    rover_mqtt_conf.password    = "hono-secret";				// Password - Leave empty for no user and password
	RoverPahoMQTT rover_mqtt = RoverPahoMQTT (	"127.0.0.1",	// MQTT-Broker host
												1883,			// MQTT-Broker port
												rover_mqtt_conf);

	// Overriding payload and topic
	char payloadMsg[] = "Hi from rover!";
    rover_mqtt.setPayload (payloadMsg, strlen(payloadMsg));
	rover_mqtt.setTopic ("rover/RoverDriving/control/2");

	// Publish is non-blocking, client disconnects afterwards
	if (0 == rover_mqtt.publish())
		printf ("Publishing successful!\n");
	else
		printf ("Publishing unsuccessful!\n");

	// Subscribe is non-blocking, works with internal callbacks
	// unsubscribe() method should be used after finished
	if (0 == rover_mqtt.subscribe())
	{
		printf ("Subscribe successful!\n");
	}
	else
	{
		printf ("Subscribe unsuccessful!\n");
	}

	// Receive the final message that is arrived from the subscribed topic
	char data[50];
	rover_mqtt.read(data);
	printf ("Received data=%s\n", data);

	// Unsubscribe and disconnect
	if (0 == rover_mqtt.unsubscribe())
	{
		printf ("Unsubscribe successful!\n");
	}
	else
	{
		printf ("Unsubscribe unsuccessful!\n");
	}

	// Sleep a bit
	r_base.sleep(5000);

	// Shutdown the rover OS and abort the application
	r_base.shutdown();

	return 1;
}
\endcode

To see the example code related to classes, you can select classes and see detailed descriptions.

\section roverdocs Rover Documentation

Link: <a href="http://app4mc-rover.github.io/rover-docs">Rover Complete Documentation</a>

\copyright Copyright(C) 2017 IDiAL Institute, Fachhochschule Dortmund

*/


/**
  *   @brief  rover Namespace contains classes to manage Rover sensors, gpio, buzzer, buttons, driving, utilities, and cloud.
  */
namespace rover
{

	/**
	  *   @brief  RoverBase class provides basic rover functions such as initialization, sleeping, and shutting down.
	  */
	class RoverBase
	{

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
			  *   @brief  Initializes the wiringPi library
			  *   @return void
			  *
			  *   \warning This function should be called once per application.
			  *   \code{.cpp}
			  *   RoverBase r_base = RoverBase();
			  *   r_base.initialize();
			  *   //.. Here comes all the rover objects and apps..
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

		private:
			/**
			  *   @brief  Initializes wiringPi library to access GPIO of Rover. This function should be called in every program run and must only be called once.
			  *   @return void
			  */
			void initializeWiringPi (void);

			/**
			 * @brief Flag for indicating whether wiringPi library initialized or not.
			 */
			int WIRINGPI_INIT_;
	};
}



#endif /* ROVER_API_HPP_ */
