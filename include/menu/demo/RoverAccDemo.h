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

//Basis Include
#include <roverapi/rover_api.hpp>

//Buzzer and button Include
#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_button.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_grooveultrasonic.hpp>

#include <menu/Menu.h>

using namespace std;
using namespace rover;

class RoverAccDemo {
public:
  uint curr_speed;
  bool running;
  RoverDriving * drv;

  RoverAccDemo(RoverDriving *drv, RoverHCSR04 * r_front, RoverHCSR04 * r_rear, RoverDisplay * disp, RoverButton * usrbtn);
  
  int run();

private:
  RoverHCSR04 * r_front;
  RoverHCSR04 * r_rear;
  RoverButton * usrbtn;
  Menu * main;

};

#endif /* _ROVER_ACC_HEADER_ */
