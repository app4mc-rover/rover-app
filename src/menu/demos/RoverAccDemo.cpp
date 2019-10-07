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

#include <menu/demo/RoverAccDemo.h>
#include <menu/Menu.h>


static const int P = 30;
static const double target_dist = 20;

bool check_button(RoverButton * btn) {
  double state = 1;
  static bool trigered = false;

  state = btn->readButton();

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}

RoverAccDemo::RoverAccDemo(RoverDriving *drv, RoverHCSR04 *r_front, RoverHCSR04 *r_rear, RoverDisplay * disp, RoverButton * btn) : curr_speed(0), running(true) {
  this->drv = drv;
  this->r_rear = r_rear;;
  this->r_front = r_front;
  this->btn = btn;
  
}

int RoverAccDemo::run() {
  double sensor_val = 0;
  double speed_to_set = 0;

  this->drv->setSpeed((int)speed_to_set);

  while(!check_button(this->btn)) {
    
    sensor_val =  this->r_front->read();
    
    speed_to_set = (sensor_val - target_dist)*P;
    
    if (speed_to_set < 0) {
        sensor_val = this->r_rear->read();
        speed_to_set = sensor_val < target_dist? 0:speed_to_set;
        this->drv->setSpeed(static_cast<int>(-speed_to_set));
        this->drv->goBackward();
    } else {
        this->drv->setSpeed(static_cast<int>(speed_to_set));
        this->drv->goForward();
    }
    
    usleep(100000);
  }

}
