/* Copyright 2018 Pedro Cuadra - pjcuadra@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _ROVER_ACC_HEADER_
#define _ROVER_ACC_HEADER_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>

#include <app/RoverDriving.h>
#include <app/RoverButtons.h>
#include <app/RoverDisplay.h>
#include <app/RoverGrooveUltrasonicSensor.h>

#include <Menu.h>

using namespace std;

class RoverAccDemo {
public:
  uint curr_speed;
  bool running;
  RoverDriving * drv;

  RoverAccDemo(RoverDriving *drv, RoverGrooveUltrasonicSensor *grv_sensor, RoverDisplay * disp, RoverButtons * btn);
  
  int run();

private:
  RoverGrooveUltrasonicSensor *grv_sensor;
  RoverButtons * btn;
  Menu * main;

};

#endif /* _ROVER_ACC_HEADER_ */
