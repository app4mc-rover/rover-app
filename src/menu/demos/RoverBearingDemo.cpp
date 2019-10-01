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
#include <math.h>

#include <demo/RoverBearingDemo.h>

static const int oled_width = 128;
static const int oled_height = 64;

RoverBearingDemo::RoverBearingDemo(RoverHmc5883L *sensor, RoverDisplay * disp, RoverButtons * btn) {
  this->sensor = sensor;
  this->disp = disp;
  this->btn = btn;

  this->sensor->read(this->ref_val);
}

inline const char * get_val_str(double val) {
  std::stringstream string_val;

  string_val << std::setprecision(2);
  string_val << std::fixed;

  string_val << val;

  return string_val.str().c_str();

}

double normalize_angle(double angle) {
  double tmp = angle;

  if (angle < 0) {
    tmp = 360 + angle;
  }

  return 360 - tmp;
}

int RoverBearingDemo::run() {
  static const int origin_x = oled_width / 3;
  static const int origin_y = oled_height / 2;
  static const int arrow_len = 30;
  static const double pi = 3.14159265;

  int tip_x = origin_x + arrow_len;
  int tip_y = origin_y;
  int arrow_tip_x_1 = tip_x;
  int arrow_tip_y_1 = tip_y;
  int arrow_tip_x_2 = tip_x;
  int arrow_tip_y_2 = tip_y;
  double sensor_val;

  this->disp->set_text_size(1);
  this->disp->set_text_color(1);

  while (!check_button()) {
    this->disp->clear_display();
    // Read Left
    this->sensor->read(sensor_val);

    sensor_val = normalize_angle(sensor_val - this->ref_val);
    this->disp->set_cursor(80, 2);
    this->disp->print(get_val_str(sensor_val));

    tip_x = origin_x;
    tip_x += std::cos(pi * sensor_val / 180) * arrow_len;
    tip_y = origin_y;
    tip_y += std::sin(pi * sensor_val / 180) * arrow_len;

    this->disp->draw_line(origin_x, origin_y, tip_x, tip_y, 1);

    arrow_tip_x_1 = tip_x;
    arrow_tip_x_1 -= std::cos(pi * (sensor_val - 45) / 180) * 5;
    arrow_tip_y_1 = tip_y;
    arrow_tip_y_1 -= std::sin(pi * (sensor_val - 45) / 180) * 5;

    // this->disp->draw_line(arrow_tip_x, arrow_tip_y, tip_x, tip_y, 1);

    arrow_tip_x_2 = tip_x;
    arrow_tip_x_2 -= std::cos(pi * (sensor_val + 45) / 180) * 5;
    arrow_tip_y_2 = tip_y;
    arrow_tip_y_2 -= std::sin(pi * (sensor_val + 45) / 180) * 5;

    this->disp->draw_triangle(arrow_tip_x_1, arrow_tip_y_1, arrow_tip_x_2, arrow_tip_y_2, tip_x, tip_y, 1, true);

    this->disp->display();
  }

}

bool RoverBearingDemo::check_button() {
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
