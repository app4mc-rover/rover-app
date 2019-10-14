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
#include <sstream>
#include <iomanip>

#include <menu/demo/RoverGrooveDemo.h>

static const int oled_width = 128;
static const int oled_height = 64;

RoverGrooveDemo::RoverGrooveDemo(RoverHCSR04 *r_front, RoverHCSR04 *r_rear, RoverDisplay * disp, RoverButton * usrbtn) {
  this->r_front = r_front;
  this->r_rear = r_rear;
  this->disp = disp;
  this->usrbtn = usrbtn;
}

inline const char * get_val_str(double val) {
  std::stringstream string_val;

  string_val << std::setprecision(2);
  string_val << std::fixed;

  string_val << val;

  return string_val.str().c_str();

}


int RoverGrooveDemo::run() {
  double sensor_val;
  static const int triangle_width = 10;
  static const int triangle_height = 20;

  this->disp->setTextSize(2);
  this->disp->setTextColor(1);

  while (!check_button()) {
    this->disp->clearDisplay();

    // Front
    sensor_val = this->r_front->read();
    // Arrow
    this->disp->drawTriangle(triangle_width,
                              0,
                              0,
                              triangle_height/2,
                              triangle_width,
                              triangle_height,
                              1);

    this->disp->setCursor(triangle_width + 4, 3);
    this->disp->print(get_val_str(sensor_val));


    // Rear
    sensor_val = this->r_rear->read();
    // Arrow
    this->disp->drawTriangle(oled_width - triangle_width,
                              oled_height - triangle_height,
                              oled_width,
                              oled_height - triangle_height/2,
                              oled_width - triangle_width,
                              oled_height,
                              1);

    this->disp->setCursor(oled_width - 72, oled_height - 17);
    this->disp->print(get_val_str(sensor_val));


    this->disp->display();
  }

}

bool RoverGrooveDemo::check_button() {
  double state = 1;
  static bool trigered = false;

  state = this->usrbtn->readButton();

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}
