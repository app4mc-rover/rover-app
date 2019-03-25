/*
 * Copyright (c) 2019 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *   Test Aruco Marker detection 
 *
 * Author:
 *   Daniel Paredes, FH Dortmund <daleonpz@gmail.com> - created 24.01.2019
 * Note: 
 *  You will find the calibration file in
 *  src/examples/calibration.yml
 *  when you want to run this application 
 *  calibration.yml should be in the same directory
 *  as the executable otherwise it won't run.
 *
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>


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
	angle[0] *= 57.29;
	angle[1] *= 57.29;
	angle[2] *= 57.29;
            
        std::cout << "Angles" << std::endl;
        std::cout << "X: " << angle[0] << '\t';
        std::cout << "Y: " << angle[1] << '\t';
        std::cout << "Z: " << angle[2] << '\n';
}


/* ------------------------------------------- */
/*   MAIN FUNCTION 		 	       */
/* ------------------------------------------- */

int main (int argc,char ** argv){
    cv::VideoCapture inputVideo(0);

    cv::Mat rvec, tvec; /* size Nx3 */
    cv::Mat cameraMatrix, distCoeffs; 

    cv::FileStorage fs("calibration.yml", cv::FileStorage::READ);
    if(!fs.isOpened()){
        std::cerr << "ERROR: Unable to read camera parameters. check calibration.yml path" << std::endl;
        return 0;
    }
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;

    int waitTime = 5; 

    if (!inputVideo.isOpened()) {
        std::cerr << "ERROR: Unable to open the camera" << std::endl;
        return 0;
     }
    inputVideo.open(0);
   
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  
    int samples = 1000; 
    while (inputVideo.grab()) {
        cv::Mat image;
        cv::Mat	imageCopy; 

        inputVideo.retrieve(image);

        image.copyTo(imageCopy); 

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;

        cv::aruco::detectMarkers(image, dictionary, corners, ids);
        cv::aruco::estimatePoseSingleMarkers(corners, 0.07, cameraMatrix, distCoeffs, rvec, tvec);

        // if at least one marker detected
        for ( int i=0; i < rvec.rows ; i++){

            for( auto i: corners){ std::cout << i<< ' '; }
            std::cout << corners.size() << std::endl;

            // corners Nx4
            float x=0, y=0; 
            for( int j=0; j<4; j++) {
                x += corners[0][j].x; 
                y += corners[0][j].y;
            }

            x /= 4;
            y /= 4;

            std::cout << "Marker Center: x " << x << ' ' ;
            std::cout << "y " << y << std::endl;

            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            cv::Mat rmat;
            float angle[3] = {0,0,0};
            float distance ;
            // this should be applied to each marker, but we will use just one for the moment       
            // we should have a matrix of angles 
            cv::Rodrigues(rvec.row(i), rmat);
            rotationMatrixToEulerAngles(rmat, angle); 
            distance = cv::norm(tvec.row(i))*100; // to cm   

            cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvec.row(i), tvec.row(i), 0.1);
        }
        cv::imshow("out", imageCopy);
        char key = (char) cv::waitKey(waitTime);
        if (key == 27)
            break;
    }   

    std::cout << "Closing the camera" << std::endl;
    inputVideo.release();
    cv::destroyAllWindows();
    std::cout << "bye!" << std::endl;
    return 0;

}



