/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Image processing task that is used for basic traffic cone detection
 *    Using OpenCV 2.4.9
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, libraries compiled for cross compilation 16.10.2017
 *    M.Ozcelikors <mozcelikors@gmail.com>, demonstrator image processing task added, 19.10.2017
 *
 *
 * Usage Instructions:
 *    1) To cross compile OpenCV, make sure you install OpenCV in your Raspberry Pi
 *    2) Then find opencv shared objects using the following command
 *    		find / -name libopencv*.so
 *    		find / -name libopencv*.so*
 *    3) Copy all of the found .so files to your C:\SysGCC\Raspberry\arm-linux-gnueabihf\sysroot\lib\arm-linux-gnueabihf (on Windows)
 *    4) Be sure to use following Linker flags: (adjust the path to your system):
 *    		-Wl,-verbose,-rpath-link,"C:\SysGCC\Raspberry\arm-linux-gnueabihf\sysroot\lib\arm-linux-gnueabihf"
 *    5) Be sure to add the libraries you are using to the linker using -l:
 *			such as ... -lopencv_core -lopencv_ml -lopencv_imgproc
 *	  6) Be sure to include the library search paths using -L:
 *	        -L"C:\SysGCC\Raspberry\arm-linux-gnueabihf\sysroot\lib\arm-linux-gnueabihf"
 *	  7) Be sure to include OpenCV and raspicam libraries below to `include directories` in gcc using -I flag.
 *
 *	  IMPORTANT!: For this task to run, camera stream or any other processes that use the raspberry pi camera should be deactivated!
 *
 */

#include <tasks/image_processing_task.h>

#include <ctime>
#include <wiringPi.h>
#include <unistd.h>
#include <libraries/timing/timing.h>
#include <api/basic_psys_rover.h>
#include <interfaces.h>
#include <pthread.h>

#include <libraries/pthread_monitoring/collect_thread_name.h>
#include <RaspberryTest.h>

#include <iostream>
#include <fstream>

/* OpenCV 2.4.9 libraries, Includes (-I) are in /workspace, include paths are specified for "g++"!! */
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

/* raspicam-0.1.3 libraries, Includes (-I) are in /workspace, include paths are specified for "g++"!! */
#include <raspicam.h>
#include <raspicam_cv.h>

using namespace cv;
using namespace std;

void *Image_Processing_Task(void *arg)
{
	timing imgproc_task_tmr;

	CollectThreadName("Image_Processing_Task");

	imgproc_task_tmr.setTaskID("ImgPr");
	imgproc_task_tmr.setDeadline(1);
	imgproc_task_tmr.setPeriod(1);
	/*
	char key;
	char* output_window_name = "Camera Output";
	char* grayscale_window_name = "Grayscale Image";
	char* thresholded_window_name = "Thresholded Image";
	char* contours_window_name = "Contours Image";

	int first_time_cmp = 1;

	int canny_lowthreshold = 20;
	int canny_highthreshold = 50;
	int width_bound = 16;//5
	int height_bound = 16;//5
	int solidity_bound = 9;
	int max_val = 200;
	int float_max_val = 30;
	int color_max_val = 256;

	//Orange
	int u_h=0;//0;//0;//0;  //0
	int u_s=177;//148;//102;//134;//120; //197
	int u_v=178;//149;//220;//212;//120; //189
	int d_h=203;//5;//256;//256; //9
	int d_s=256;//256; //256
	int d_v=256;//256; //256

	int up_aspectRatio = 26;
	int down_aspectRatio = 15;
	int aspect_max_val = 50;
	RNG rng(12345);

	int detections_w[10];
	int detections_h[10];
	int detections_x[10];
	int detections_y[10];
	double  detections_wh[10];
	int detection_count = 0;

	int send_flag = 0;

	namedWindow(output_window_name, CV_WINDOW_AUTOSIZE);
	namedWindow(grayscale_window_name, 100);
	namedWindow(thresholded_window_name, 100);
	namedWindow(contours_window_name, 100);

	moveWindow(grayscale_window_name, 20,20);
	moveWindow(thresholded_window_name, 20, 250);
	moveWindow(contours_window_name, 20, 600);


	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	raspicam::RaspiCam_Cv Camera;
	//Camera.set (CV_CAP_PROP_FORMAT, CV_8UC3);//CV_BGR2HSV); // Original format in Raspicam -> CV_8UC1);
	if (!Camera.open()){
		printf("Camera failed to open!\n");
		abort();
	}

	Mat frame;
	Mat frameRGB;
	Mat imgGrayScale;
	Mat imgHsv;
	Mat imgRedThresh;
	Mat imgWhiteThresh;
	Mat imgResultingThresh;
	Mat edges;
	vector< vector<Point> > contours;
	vector< Vec4i > hierarchy;
	vector<Point> approx;
	vector<Point> tempCont;
	vector<vector<Point> > tempVecCont;
	double contPeri;
	Rect bBox;
	double aspectRatio;

	double area;
	double hullArea;
	double solidity;

	int opening_length;
	int opening_midpoint;

	double maximum ;
	double second_maximum ;
	double third_maximum;
	int maximum_idx ;
	int second_maximum_idx ;
	int third_maximum_idx;
	double second_last_maximum;
	double last_maximum;

	int counter = 0;
	int counter_max = 1;
	//createTrackbar("canny low Threshold: ", output_window_name, &canny_lowthreshold, max_val, canny);
	//createTrackbar("canny high Threshold: ", output_window_name, &canny_highthreshold, max_val, canny);
	//createTrackbar("height: ", output_window_name, &height_bound, float_max_val, canny);
	//createTrackbar("width: ", output_window_name, &width_bound, float_max_val, canny);
	//createTrackbar("solidity: ", output_window_name, &solidity_bound, float_max_val, canny);
	//createTrackbar("Up AspectRatio: ", output_window_name, &up_aspectRatio, aspect_max_val, canny);
	//createTrackbar("Down AspectRatio: ", output_window_name, &down_aspectRatio, aspect_max_val, canny);
	//createTrackbar("Up H: ", output_window_name, &u_h, color_max_val, canny);
	//createTrackbar("Up S: ", output_window_name, &u_s, color_max_val, canny);
	//createTrackbar("Up v: ", output_window_name, &u_v, color_max_val, canny);
	//createTrackbar("Down H: ", output_window_name, &d_h, color_max_val, canny);
	//createTrackbar("Down S: ", output_window_name, &d_s, color_max_val, canny);
	//createTrackbar("Down v: ", output_window_name, &d_v, color_max_val, canny);
*/

	while (1)
	{
		imgproc_task_tmr.recordStartTime();
		imgproc_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------
/*
		//Capture image with Raspicam_CV
		Camera.grab();
		Camera.retrieve(frame);  //cap >> frame

		//cv::imwrite("raspicam_img.jpg", frame);

		//printf("Starting...\n");
		//key = waitKey(10);
		//cout <<(int) char(key) << endl;
		//if(char(key) == 27){
		//    break;
		//}
		// if(char(key) == 10){

		frameRGB.create(frame.size(), frame.type());
		cvtColor(frame, frameRGB, CV_BGR2RGB);

		imgHsv.create(frameRGB.size(), frameRGB.type());
		cvtColor(frameRGB, imgHsv, CV_RGB2HSV);//HSV

		//cv::imwrite("raspicam_img.jpg", imgHsv);

		inRange(imgHsv, Scalar(u_h, u_s, u_v), Scalar(d_h, d_s, d_v), imgRedThresh);

		//converting the original image into grayscale
		imgGrayScale.create(frame.size(), frame.type());
		cvtColor(frame, imgGrayScale, CV_BGR2GRAY);
		bitwise_and(imgRedThresh, imgGrayScale, imgGrayScale);

		// Floodfill from point (0, 0)
		Mat im_floodfill = imgGrayScale.clone();
		floodFill(im_floodfill, cv::Point(0,0), Scalar(255));

		// Invert floodfilled image
		Mat im_floodfill_inv;
		bitwise_not(im_floodfill, im_floodfill_inv);

		// Combine the two images to get the foreground.
		imgGrayScale = (imgGrayScale | im_floodfill_inv);

		GaussianBlur(imgGrayScale, imgGrayScale, Size(7,7), 1.5, 1.5);

		//imshow(grayscale_window_name, imgGrayScale);

		edges.create(imgGrayScale.size(), imgGrayScale.type());
		Canny(imgGrayScale, edges, canny_lowthreshold, canny_highthreshold, 3);

		findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
		Mat drawing = Mat::zeros(edges.size(), CV_8UC3);

		//For all contours
		for(int i=0 ; i<contours.size() ; i++){
			contPeri = arcLength(contours.at(i), true);
			approxPolyDP(contours.at(i), approx,  0.01 * contPeri, true);

			//if(approx.size()>=7 && approx.size()<=9){
			bBox = boundingRect(approx);
			aspectRatio = bBox.width/bBox.height;

			area = contourArea(contours.at(i));
			convexHull(contours.at(i), tempCont);
			hullArea = contourArea(tempCont);
			solidity = area / hullArea;

			// cout << "contour : " << approx.size() << endl;
			// cout << bBox.width << "::" << bBox.height << "::" << solidity  << "::" << aspectRatio <<endl;

			tempVecCont.clear();
			tempVecCont.push_back(approx);

			if(bBox.width > width_bound && bBox.height > height_bound && solidity > solidity_bound/10 )
			{
				//Find detections
				//printf("%d\t%d\n",bBox.width, bBox.height);
				detections_x[detection_count] = bBox.x;
				detections_y[detection_count] = bBox.y;
				detections_w[detection_count] = bBox.width;
				detections_h[detection_count] = bBox.height;
				detections_wh[detection_count] = bBox.width*bBox.height;
				detection_count = detection_count + 1;

				//Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
				//drawContours(drawing, tempVecCont, 0, color, 2, 8, hierarchy, 0, Point());
				//rectangle( drawing, Point( bBox.x, bBox.y), Point( bBox.x+bBox.width, bBox.y+bBox.height), Scalar( 0, 55, 255 ), +4, 4 );
			}
			//}

		}
		//First we shall select the biggest two boxes, having biggest w x h.
		maximum = detections_wh[0];
		second_maximum = detections_wh[0];
		third_maximum = detections_wh[0];
		maximum_idx = 0;
		second_maximum_idx = 0;
		third_maximum_idx = 0;
		int c;
		for (c = 0; c < detection_count; c++)
		{
			if (detections_wh[c] > maximum)
			{
				third_maximum = second_maximum;
				third_maximum_idx = second_maximum_idx;
				second_maximum = maximum;
				second_maximum_idx = maximum_idx;
				maximum  = detections_wh[c];
				maximum_idx = c;
			}
		}

		if(maximum > 10000 || second_maximum > 10000 || third_maximum > 10000) //was 3000
		{
			send_flag = 1;
			//Clear detection width*height array after command is sent
			for (c = 0; c<detection_count; c++)
			{
				detections_wh[c] = 0;
			}
		}
		else
		{
			ofstream myfile;
			myfile.open("../../logs/image_processing/detection.inc");
			myfile << "undetected";
			myfile.close();
		}
		second_last_maximum = second_maximum;
		last_maximum = maximum;


		if(send_flag == 1){

			printf("CONTOUR DETECTED\n");
			counter = 0;
			send_flag = 0;
		}



		counter++;
		*/

		//Task content ends here -------------------------------------------------

		imgproc_task_tmr.recordEndTime();
		imgproc_task_tmr.calculateExecutionTime();
		imgproc_task_tmr.calculateDeadlineMissPercentage();
		imgproc_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&imgproc_task_ti_l);
			imgproc_task_ti.deadline = imgproc_task_tmr.getDeadline();
			imgproc_task_ti.deadline_miss_percentage = imgproc_task_tmr.getDeadlineMissPercentage();
			imgproc_task_ti.execution_time = imgproc_task_tmr.getExecutionTime();
			imgproc_task_ti.period = imgproc_task_tmr.getPeriod();
			imgproc_task_ti.prev_slack_time = imgproc_task_tmr.getPrevSlackTime();
			imgproc_task_ti.task_id = imgproc_task_tmr.getTaskID();
			imgproc_task_ti.start_time = imgproc_task_tmr.getStartTime();
			imgproc_task_ti.end_time = imgproc_task_tmr.getEndTime();
		pthread_mutex_unlock(&imgproc_task_ti_l);
		imgproc_task_tmr.sleepToMatchPeriod();
	}

	// Destroy the screens
	/*destroyWindow(output_window_name);
	destroyWindow(thresholded_window_name);
	destroyWindow(grayscale_window_name);
	destroyWindow(contours_window_name);*/

	//Close raspicam
	//cap.release(); --> In webcam
	/*Camera.release();*/

	/* the function must return something - NULL will do */
	return NULL;
}
