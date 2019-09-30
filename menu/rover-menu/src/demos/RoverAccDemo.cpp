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

#include <demo/RoverAccDemo.h>

#include <Menu.h>

static const int P = 30;
static const double target_dist = 20;

bool check_button(RoverButtons * btn) {
  double state = 1;
  static bool trigered = false;

  btn->read(user_button, state);

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}

RoverAccDemo::RoverAccDemo(RoverDriving *drv, RoverGrooveUltrasonicSensor *grv_sensor, RoverDisplay * disp, RoverButtons * btn) : curr_speed(0), running(true) {
  this->drv = drv;
  this->grv_sensor = grv_sensor;
  this->btn = btn;
  
}

int RoverAccDemo::run() {
  double sensor_val = 0;
  double speed_to_set = 0;

  this->drv->setspeed((int)speed_to_set);

  while(!check_button(this->btn)) {
    
    this->grv_sensor->read(rover_sensor_id::front, sensor_val);
    
    speed_to_set = (sensor_val - target_dist)*P;
    
    if (speed_to_set < 0) {
        this->grv_sensor->read(rover_sensor_id::rear, sensor_val);
        speed_to_set = sensor_val < target_dist? 0:speed_to_set;
        this->drv->setspeed(static_cast<int>(-speed_to_set));
        this->drv->gobackward();
    } else {
        this->drv->setspeed(static_cast<int>(speed_to_set));
        this->drv->goforward();
    }
    
    usleep(100000);
  }

}
