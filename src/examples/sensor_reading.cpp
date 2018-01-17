/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover sensor reading example
 *
 * Author:
 *    M. Ozcelikors, FH Dortmund <mozcelikors@gmail.com> - 11.01.2018
 *
 */

#include <stdio.h>

//Basis Include
#include <roverapi/rover_api.hpp>

//Sensor APIs Include
#include <roverapi/rover_grooveultrasonic.hpp>
#include <roverapi/rover_gy521.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_hmc5883l.hpp>
#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/rover_qmc5883l.hpp>
#include <roverapi/rover_dht22.hpp>

//Using rover namespace from Rover API
using namespace rover;

// Main function
int main()
{
    printf("Started roverapp sensor_reading example.\n");
    
    //This initialization is a one time only must-call before every rover application.
    RoverBase r_base = RoverBase();
    r_base.initialize();
    
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
    RoverGY521 r_accel = RoverGY521();
    
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
    // Set up GY521 accelerometer
    r_accel.initialize();
    
    //Read from sensors
    printf ("Ultrasonic = [%f %f]\n",   r_front.read(),
                                        r_rear.read());
    printf ("Infrared = [%f %f %f %f]\n",   r_infrared0.read(), // Rear-right
                                            r_infrared1.read(), // Rear-left
                                            r_infrared2.read(), // Front-right
                                            r_infrared3.read());// Front-left
    printf ("Temperature = %f\n",   r_dht22.readTemperature());
    printf ("Humidity = %f\n",      r_dht22.readHumidity());
    printf ("Bearing with HMC5883L = %f\n",     r_hmc.read());
    // or printf ("Bearing with QMC5883L = %f\n",       r_qmc.read());
    printf ("GY521 AccelX = %d\n", r_accel.getAccelX());
    printf ("GY521 AccelY = %d\n", r_accel.getAccelY());
    printf ("GY521 AccelZ = %d\n", r_accel.getAccelZ());
    printf ("GY521 GyroX = %d\n", r_accel.getGyroX());
    printf ("GY521 GyroY = %d\n", r_accel.getGyroY());
    printf ("GY521 GyroZ = %d\n", r_accel.getGyroZ());
    printf ("GY521 AngleX = %f\n", r_accel.getAngleX());
    printf ("GY521 AngleY = %f\n", r_accel.getAngleY());
    printf ("GY521 AngleZ = %f\n", r_accel.getAngleZ());
    printf ("GY521 Bearing = %f\n", r_accel.read());

	printf("Exiting.\n");

	return 0;
}
