/*
 * Copyright (c) 2016 Eclipse Foundation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Authors:
 *    Gaël Blondelle - initial implementation
 * Contributors:
 */

#include <roverapi/basic_psys_rover.h>

#include <wiringPi.h>
#include <mcp3004.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>


static int led_status = 0;
static int i2c_th02_fd = 0;

void init(){
    //wiringPiSetup () ;
#if !SIMULATOR

    pinMode (ENABLE_MOTOR_LEFT, OUTPUT) ;
    digitalWrite (ENABLE_MOTOR_LEFT, HIGH) ;
    pinMode (ENABLE_MOTOR_RIGHT, OUTPUT) ;
    digitalWrite (ENABLE_MOTOR_RIGHT, HIGH) ;
    pinMode (DIRECTION_PIN_LEFT, OUTPUT) ;
    pinMode (DIRECTION_PIN_RIGHT, OUTPUT) ;


    softPwmCreate (SOFT_PWM_ENGINE_LEFT, 0, FULL_SPEED) ;
    softPwmCreate (SOFT_PWM_ENGINE_RIGHT, 0, FULL_SPEED) ;

    pinMode (FLASH_LIGHT_LED, OUTPUT) ;

    // Init the analog digital converter
    mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels

    // Init the I2C interface for device 0x40 which is the id of the temperature/humidity sensor
    i2c_th02_fd = wiringPiI2CSetup (0x40);
#endif
}

float getDistance(int channel){
    float x = 0;
#if !SIMULATOR
    float y = analogRead (BASE+channel);
#else
    float y = 0;
#endif

    // 1/cm to output voltage is almost linear between
    // 80cm->0,4V->123
    // 6cm->3,1V->961
    // => y=5477*x+55 => x= (y-55)/5477
    if (y<123){
        x=100.00;
    } else {
        float inverse = (y-55)/5477;
        printf("inverse=%f\n",inverse);
        // x is the distance in cm
        x = 1/inverse;
    }

    printf("Distance channel row data %d:%f\n",channel,y);
    printf("Distance channel (cm) %d:%f\n",channel,x);

    return x;
}

float getTemperature(){
    float x = 0;

    unsigned char command[2]= {0};

    command[0]=0x03;
    command[1]=0x11;

    printf("fd:%d\n",i2c_th02_fd);

    //	wiringPiI2CWriteReg8(i2c_th02_fd, 0x03, 0x01);

#if !SIMULATOR
    write(i2c_th02_fd,command,2);
#endif

    // Poll RDY (D0) in STATUS (register 0) until it is low (=0)
    int status = -1;
    delay (30);
#if !SIMULATOR
    while ((status & 0x01) != 0) {
        status = wiringPiI2CReadReg8(i2c_th02_fd,0);
        printf("Status:%d\n",status);
    }
#endif

    // Read the upper and lower bytes of the temperature value from
    // DATAh and DATAl (registers 0x01 and 0x02), respectively
    unsigned char buffer[3]= {0};

#if !SIMULATOR
    read(i2c_th02_fd, buffer, 3);
#endif

    int dataH = buffer[1] & 0xff;
    int dataL = buffer[2] & 0xff;

    //	int dataH = wiringPiI2CRead(i2c_th02_fd)&0xff;
    //	int dataL = wiringPiI2CRead(i2c_th02_fd)&0xff;
    printf("dataH:%02X\n",dataH);
    printf("dataL:%02X\n",dataL);

    x = (dataH * 256 + dataL) >> 2;
    printf("Temperature raw:%d\n",(int)x);
    x = (x / 32) - 50;

    printf("Temperature:%f\n",x);

    return x;
}

float getHumidity(){
    float x = 0;

    unsigned char command[2]= {0};

    command[0]=0x03;
    command[1]=0x01;

    printf("fd:%d\n",i2c_th02_fd);

    //	wiringPiI2CWriteReg8(i2c_th02_fd, 0x03, 0x01);

#if !SIMULATOR
    write(i2c_th02_fd,command,2);
#endif

    // Poll RDY (D0) in STATUS (register 0) until it is low (=0)
    int status = -1;
    delay (30);
#if !SIMULATOR
    while ((status & 0x01) != 0) {
        status = wiringPiI2CReadReg8(i2c_th02_fd,0);
        printf("Status:%d\n",status);
    }
#endif

    // Read the upper and lower bytes of the temperature value from
    // DATAh and DATAl (registers 0x01 and 0x02), respectively
    unsigned char buffer[3]= {0};

#if !SIMULATOR
    read(i2c_th02_fd, buffer, 3);
#endif

    int dataH = buffer[1] & 0xff;
    int dataL = buffer[2] & 0xff;

    //	int dataH = wiringPiI2CRead(i2c_th02_fd)&0xff;
    //	int dataL = wiringPiI2CRead(i2c_th02_fd)&0xff;
    printf("dataH:%02X\n",dataH);
    printf("dataL:%02X\n",dataL);

    x = (dataH * 256 + dataL) >> 4;
    printf("Humidity raw:%d\n",(int)x);
    x = (x / 16) - 24;

    printf("Humidity:%f\n",x);

    return x;
}


/*!
 * @func differential_drive 
 * @brief Calculate the wheel speed for differential
 *          drive system
 *
 * @param[in] angular_speed  Vehicule angular speed. 
 * @param[in] linear_speed   Vehicule linear speed 
 *
 * @return none .
 */
void 
differential_drive( float angular_speed , float linear_speed )
{
    int speed_right_wheel;
    int speed_left_wheel;

    // NOTE: Distance between wheels
    // is two low, in theory is 20cm
    float distance_btw_wheels = 0.2; 
    float inv_wheel_d =  16; // 1/wheel_diameter
    speed_right_wheel = inv_wheel_d*(2*linear_speed + distance_btw_wheels*angular_speed*360);
    speed_left_wheel = inv_wheel_d*(2*linear_speed - distance_btw_wheels*angular_speed*360);


    int direction_left = (speed_left_wheel >= 0)?FORWARD: BACKWARD ;
    int direction_right = (speed_right_wheel >= 0)?FORWARD: BACKWARD;

#if SIMULATOR
    printf("Speed \n");
    printf("linear_speed %f \t", linear_speed);
    printf("angular_speed %f \n", angular_speed);
    printf("Right wheel: %i \t", speed_right_wheel/4);
    printf("Left wheel: %i \n", speed_left_wheel/4);
#endif 


    /* TODO: scale speed to pwm values */
    // pwd = speed / scale_factor;

    // 4 has no math foundation 
    int scale_factor = 4;
    runside(LEFT, direction_left, (speed_left_wheel/scale_factor) );
    runside(RIGHT, direction_right, (speed_right_wheel/scale_factor) );

}

void runside(int side, int direction, int speed){
#if !SIMULATOR
    // POLOLU_2756
    if (side==LEFT){
        if (direction>0) {
            digitalWrite (DIRECTION_PIN_LEFT, HIGH) ;
        } else if (direction<0) {
            digitalWrite (DIRECTION_PIN_LEFT, LOW) ;
        } else return;
        //softPwmCreate (SOFT_PWM_ENGINE_LEFT, 0, FULL_SPEED);
        softPwmWrite (SOFT_PWM_ENGINE_LEFT, speed) ;
        //pwmWrite (SOFT_PWM_ENGINE_LEFT, speed);
        //digitalWrite(SOFT_PWM_ENGINE_LEFT, HIGH);
    } else if (side==RIGHT){
        if (direction>0) {
            digitalWrite (DIRECTION_PIN_RIGHT, HIGH) ;

        } else if (direction<0) {
            digitalWrite (DIRECTION_PIN_RIGHT, LOW) ;
        } else return;
        //softPwmCreate (SOFT_PWM_ENGINE_LEFT, 0, FULL_SPEED);
        softPwmWrite (SOFT_PWM_ENGINE_RIGHT, speed) ;
        //pwmWrite (SOFT_PWM_ENGINE_LEFT, speed);
        //softToneWrite (2, 400);
        //digitalWrite(SOFT_PWM_ENGINE_RIGHT, HIGH);
}
#else
    printf("Run side - side: %d, direction: %d, speed %d\n", side, direction, speed);
#endif
}



void go (int direction, int speed){
    runside (LEFT, direction, speed);
    runside (RIGHT, direction, speed);

    //delay (DEFAULT_DELAY) ;
}

void turn (int direction, int side, int speed){
    if (side==LEFT){
        //		runside (LEFT, -direction, speed);
        runside (LEFT, direction, 0);
        runside (RIGHT, direction, speed);
    }
    if (side==RIGHT){
        runside (LEFT, direction, speed);
        //		runside (RIGHT, -direction,speed);
        runside (RIGHT, direction, 0);
    }
    //delay (DEFAULT_DELAY) ;
}


void turnOnSpot (int direction, int side, int speed){
    if (side==LEFT){
        runside (RIGHT, -direction, speed);
        //		runside (LEFT, direction, 0);
        runside (LEFT, direction, speed);
    }
    if (side==RIGHT){
        runside (RIGHT, direction, speed);
        runside (LEFT, -direction,speed);
        //		runside (RIGHT, direction, 0);
    }
    //delay (DEFAULT_DELAY) ;
}


void stop(){
    runside (LEFT, FORWARD,0);
    runside (RIGHT, FORWARD,0);
}

void toggle_light(){
#if !SIMULATOR
    digitalWrite (FLASH_LIGHT_LED, ++led_status % 2) ;
#endif
}
