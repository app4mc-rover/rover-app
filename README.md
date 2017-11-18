# rover-app
roverapp Software (C/C++) for APP4MC Rover project

![API Documentation Link](https://travis-ci.org/app4mc-rover/rover-app.svg?branch=master)

API documentation link: https://app4mc-rover.github.io/rover-app

Comprehensive documentation link: https://app4mc-rover.github.io/rover-docs

**Roverapp** complete feature list is given below:

* Multi-threaded, schedulable and traceable embedded software.
* Cloud communication to Hono 0.5-M9 infrastructure using REST API, using customly created **hono_interaction** library.
* Utilized drivers for Linux modules such as bluetooth (bluetooth-dev).
* I2C drivers and applications (threads) for OLED display, SRF02 ultrasonic sensor, HMC5883L magnetometer, GY-521Y accelerometer, etc.
* Temperature and humidity measurement using DHT22 sensor.
* Reactive implementations for TCP socket server and TCP socket client, with proper JSON formatted data for communication.
* OLED display application that is able to display bluetooth, Hono cloud, ethernet, wireless interface, and internet with the help of **status_library** library.
* OpenCV 2.4.9 utilization and image processing application (currently Traffic cone detection).
* SHARP Analog Proximity measurement sensor interfacing and implementations.
* SRF-02, HCSR-04, and Grove Ultrasonic sensor interfacing and implementations.
* Motor driving implementations.
* Timing measurement implementations with the help of **timing** library.
* CPU core utilization measurement implementation.
* Adaptive Cruise Control behavior implementation.
* Parking behavior implementation.
* Booth mode implementations.
* Implementations for bluetooth-based driving from Android phones.

