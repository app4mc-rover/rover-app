/*
 * Copyright (c) 2019 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *      Pose based visual servoing with Aruco markers
 *
 * Author:
 *    Daniel Paredes <daleonpz@gmail.com> - 04.04.2019
 *
 */
// TODO: only works when the marker is not upside down  

#include <stdio.h>
#include <math.h>       /* fabs, cos */

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>

//Basis Include
#include <roverapi/rover_api.hpp>

//Sensor APIs Include
#include <roverapi/rover_grooveultrasonic.hpp>
#include <roverapi/rover_gy521.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_hmc5883l.hpp>
#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/rover_driving.hpp>

//Using rover namespace from Rover API
using namespace rover;

/* ------------------------------------------- */
/*   GLOBAL VARIABLES:                         */
/* ------------------------------------------- */
RoverBase r_base; /* Rover API */
RoverDriving r_driving;  /* Rover driving service */
RoverGY521 r_accel; /* gyro and accelerometer */

/* Ultrasonic sensors */
RoverHCSR04 r_front = RoverHCSR04(ROVER_FRONT);
RoverHCSR04 r_rear  = RoverHCSR04(ROVER_REAR); 

/* Infrared sensors */
RoverInfraredSensor r_infrared0 = RoverInfraredSensor(ROVER_REAR_RIGHT);
RoverInfraredSensor r_infrared1 = RoverInfraredSensor(ROVER_REAR_LEFT);
RoverInfraredSensor r_infrared2 = RoverInfraredSensor(ROVER_FRONT_RIGHT);
RoverInfraredSensor r_infrared3 = RoverInfraredSensor(ROVER_FRONT_LEFT);

/* ------------------------------------------- */
/*   FUNCTION: controlLoopPBVS */
/* ------------------------------------------- */
/*
 *  moves Rover to some position in a smooth
 *  way 
 *
 *
 * tvec_des: desired translation vector
 * rvec:   rotation vector angle/axis parameterization
 * tvec:   transtation vector
 * speed:   estimated linear speed 
 * angular_speed: estimated angular speed 
 * gain:    control gain
 *
 * assuming that desired position is parallel to the camera
 * at some distance d = norm(tdes) from it
 * 
 * Data type: CV_64FC1
 */

void controlLoopPBVS(const cv::Mat tdes, const cv::Mat tvec, const cv::Mat rvec,
        cv::Mat &speed, cv::Mat &angular_speed, int gain){

    // skew symmetric matrix
    cv::Mat t_x = cv::Mat::zeros(3,3, CV_64FC1);

    t_x.at<double>(0,1) = -tvec.at<double>(2);
    t_x.at<double>(0,2) = tvec.at<double>(1);
    t_x.at<double>(1,0) = tvec.at<double>(2);
    t_x.at<double>(1,2) = -tvec.at<double>(0);
    t_x.at<double>(2,0) = -tvec.at<double>(1);
    t_x.at<double>(2,1) = tvec.at<double>(0);

    speed =  -gain*(tdes - tvec + t_x*rvec);
    angular_speed = -gain*rvec;
}


/* ------------------------------------------- */
/*   FUNCTION: controller */
/* ------------------------------------------- */
/*
 *  moves Rover to some position in a smooth
 *  way 
 *  
 *  distDesired: set desired distance to the marker
 *  _tvec: translation vector
 *  _rvec: rotation vector
 */

void controller(const float distDesired, const cv::Mat _tvec, const cv::Mat _rvec){

    cv::Mat speed; // 3x1
    cv::Mat angular_speed; // 3x1
    int gain = 1;

    /* Convert from CV_64FC3 to CV_64FC1 */
    // I tried with convertTo method, but didn't work
    cv::Mat tdes = cv::Mat::zeros(3,1, CV_64FC1);
    tdes.at<double>(2) = distDesired;

    cv::Mat t = cv::Mat::zeros(3,1, CV_64FC1);
    for( int i=0; i<3; i++) t.at<double>(i) = _tvec.at<double>(i)*100; // to cm

    cv::Mat r = cv::Mat::zeros(3,1, CV_64FC1);
    for( int i=0; i<3; i++) r.at<double>(i) = _rvec.at<double>(i);
    
    controlLoopPBVS( tdes, t, r, speed, angular_speed, gain);

    float Vz = speed.at<double>(2);
    float w_y = angular_speed.at<double>(2);

    r_driving.differentialDrive(w_y, Vz);
}

/* ------------------------------------------- */
/*   FUNCTION: rotationMatrixToEulerAngles     */
/* ------------------------------------------- */
/*
 *  computes the Euler angles from a rotation matrix:
 *
 *  R: rotation matrix
 *  angle: vector with the angles (x, y ,z)
 * 
 */

void rotationMatrixToEulerAngles(cv::Mat &R, float *angle){
    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

    bool singular = sy < 1e-6; 
    float x, y, z;

    if (!singular) {
        angle[0] = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        angle[1] = atan2(-R.at<double>(2,0), sy);
        angle[2] = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else {
        angle[0] = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        angle[1] = atan2(-R.at<double>(2,0), sy);
        angle[2] = 0;
    }

}

/* ------------------------------------------- */
/*   ROVER TOOLS INITIALIZATION                */
/* ------------------------------------------- */
void  Initialize_API(){
    r_base = RoverBase();
    r_base.initialize();
}

void Initialize_MOTORS(){
    r_driving = RoverDriving();
    r_driving.initialize();
    r_driving.setSpeed(HIGHEST_SPEED);
}

void Initialize_Sensors(){
    r_accel = RoverGY521();

    // Set up ultrasonic sensors
    r_front.initialize();
    r_rear.initialize();

    // Set up infrared sensors
    r_infrared0.initialize();
    r_infrared1.initialize();
    r_infrared2.initialize();
    r_infrared3.initialize();

    // Set up GY521 accelerometer
    r_accel.initialize();
    r_base.sleep(100);
}

/* ------------------------------------------- */
/*   MAIN FUNCTION 	                           */
/* ------------------------------------------- */
int main() {

    /**************************************/
    /* Rover API set up                   */
    /**************************************/ 
    puts("Initializing Rover API..");
    //This initialization is a one time only must-call before every rover application.
    Initialize_API();    

    puts("Initializing Motors and Sensors..");
    Initialize_MOTORS();
    Initialize_Sensors();

    /**************************************/
    /*  OpenCV and Camera Configuration   */
    /**************************************/ 
    puts("Setting up PiCamera..");
    cv::VideoCapture inputVideo(0);

    cv::Mat image;
    cv::Mat rvec, tvec; /* size Nx3 */
    cv::Mat cameraMatrix, distCoeffs; 
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f> > corners;

    cv::FileStorage fs("calibration.yml", cv::FileStorage::READ);
    if(!fs.isOpened()){
        std::cerr << "ERROR: Unable to read camera parameters. check calibration.yml path" << std::endl;
        return 0;
    }
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;

    float setPoint;
    fs["image_width"] >> setPoint; setPoint /= 2;

    if (!inputVideo.isOpened()) {
        std::cerr << "ERROR: Unable to open the camera" << std::endl;
        std::cerr << "try: sudo modprobe bcm2835-v4l2" << std::endl;
        return 0;
    }

    inputVideo.open(0);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    /**************************************/
    /*  Variables for the infinity loop   */
    /**************************************/ 

    int waitTime = 5; // just for video checking
    cv::Mat imageCopy; // just for video checking
    cv::Mat rmat; // Rotation Matrix 

    int samples = 30 ; /* to filter estimated angle */
    int isThereAMarker = 10; 
    int counterNoMarker = 0;
    float angles[3] = {0,0,0};

    // Variables for control loop, 'sensors'
    float angleYaxis = 0 ;
    float distance = 0;
    float centroidPosition = 0;

    puts("Ready to grab frames..");

    /********************************************/
    /*      LOOP                                */
    /********************************************/
    while (1){
        if ( !inputVideo.read(image) ){
            std::cerr << "ERROR: Unable to read frame" << std::endl;
            return 0;
        }

        cv::aruco::detectMarkers(image, dictionary, corners, ids);

        if (ids.size() > 0){ //if at least one marker is detected

            counterNoMarker = 0;

            cv::aruco::estimatePoseSingleMarkers(corners, 0.07, cameraMatrix, distCoeffs, rvec, tvec);

            for (int i=0; i<rvec.rows; i++){ // For each marker
                cv::Rodrigues(rvec.row(i), rmat);

                // angles should be an array Nx3, and distance Nx1, N = number of markers
                // but right now: angles 1x3 and distance 1x1
                rotationMatrixToEulerAngles(rmat, angles);
                angleYaxis = angles[1]; // We only care angles in Y axis

                distance = cv::norm(tvec.row(i));

                centroidPosition = 0;
                for( int j=0; j<4; j++) {
                    centroidPosition += corners[i][j].x; // y+= corners[0][j].y;
                }
                centroidPosition /= 4; // y /= 4;

                printf("Estimate angle %f\n", angleYaxis*57.29);
                printf("Estimate distance %f\n", distance);
                printf("Current Center %f\n", centroidPosition);

                controller( 0.15, tvec.row(i), rvec.row(i)); // 15 cm
            }
        }
        else {
            counterNoMarker++;
            if ( counterNoMarker > isThereAMarker )
                r_driving.stopRover();
        }
    }   

    std::cout << "Closing the camera" << std::endl;
    inputVideo.release();
    cv::destroyAllWindows();
    std::cout << "bye!" << std::endl;
    return 0;
}

