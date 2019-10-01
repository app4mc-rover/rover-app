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

#include <demo/RoverGrooveDemo.h>

static const int oled_width = 128;
static const int oled_height = 64;

RoverGrooveDemo::RoverGrooveDemo(RoverGrooveUltrasonicSensor *grv_sensor, RoverDisplay * disp, RoverButtons * btn) {
  this->grv_sensor = grv_sensor;
  this->disp = disp;
  this->btn = btn;
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

  this->disp->set_text_size(2);
  this->disp->set_text_color(1);

  while (!check_button()) {
    this->disp->clear_display();

    // Front
    this->grv_sensor->read(rover_sensor_id::front, sensor_val);
    // Arrow
    this->disp->draw_triangle(triangle_width,
                              0,
                              0,
                              triangle_height/2,
                              triangle_width,
                              triangle_height,
                              1,
                              true);

    this->disp->set_cursor(triangle_width + 4, 3);
  	this->disp->print(get_val_str(sensor_val));


    // Rear
    this->grv_sensor->read(rover_sensor_id::rear, sensor_val);
    // Arrow
    this->disp->draw_triangle(oled_width - triangle_width,
                              oled_height - triangle_height,
                              oled_width,
                              oled_height - triangle_height/2,
                              oled_width - triangle_width,
                              oled_height,
                              1,
                              true);

    this->disp->set_cursor(oled_width - 72, oled_height - 17);
    this->disp->print(get_val_str(sensor_val));


    this->disp->display();
  }

}

bool RoverGrooveDemo::check_button() {
  double state = 1;
  static bool trigered = false;

  this->btn->read(user_button, state);

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}
