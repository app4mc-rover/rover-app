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
 *    Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, libraries compiled for cross compilation 16.10.2017
 *    M.Ozcelikors <mozcelikors@gmail.com>, demonstrator image processing task added, 19.10.2017
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
 *	  7) Be sure to include OpenCV include directories in gcc using -I flag.
 *
 */

#ifndef TASKS_IMAGE_PROCESSING_TASK_H_
#define TASKS_IMAGE_PROCESSING_TASK_H_

/* Interfaces */
void *Image_Processing_Task (void *arg);



#endif /* TASKS_IMAGE_PROCESSING_TASK_H_ */
