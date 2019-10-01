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

#include <demo/RoverInfraredDemo.h>

static const int oled_width = 128;
static const int oled_height = 64;

RoverInfraredDemo::RoverInfraredDemo(RoverInfraredSensor *inf_sensor, RoverDisplay * disp, RoverButtons * btn) {
  this->inf_sensor = inf_sensor;
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

int RoverInfraredDemo::run() {
  static const int triangle_width = 10;
  static const int triangle_height = 10;
  double sensor_val;


  this->disp->set_text_size(1);
  this->disp->set_text_color(1);

  while (!check_button()) {
    this->disp->clear_display();

    // Draw all triangles
    // Up-left
    this->disp->draw_triangle(0,
                              0,
                              0,
                              triangle_height,
                              triangle_width,
                              0,
                              1,
                              true);
    // Bottom-left
    this->disp->draw_triangle(0,
                              oled_height,
                              triangle_width,
                              oled_height,
                              0,
                              oled_height - triangle_height,
                              1,
                              true);

    // Up-right
    this->disp->draw_triangle(oled_width,
                              0,
                              oled_width,
                              triangle_height,
                              oled_width - triangle_width,
                              0,
                              1,
                              true);
    // Buttom-right
    this->disp->draw_triangle(oled_width,
                              oled_height,
                              oled_width,
                              oled_height - triangle_height,
                              oled_width - triangle_width,
                              oled_height,
                              1,
                              true);


    // Read Left
    this->inf_sensor->read(rover_sensor_id::rear_right, sensor_val);
    this->disp->set_cursor(80, 2);
    this->disp->print(get_val_str(sensor_val));

    // Read right
    this->inf_sensor->read(rover_sensor_id::rear_left, sensor_val);
    this->disp->set_cursor(80,  oled_height - (8 + 2));
    this->disp->print(get_val_str(sensor_val));


    // Front right
    this->inf_sensor->read(rover_sensor_id::front_right, sensor_val);
    this->disp->set_cursor(triangle_width + 2, 2);
  	this->disp->print(get_val_str(sensor_val));

    // Front left
    this->inf_sensor->read(rover_sensor_id::front_left, sensor_val);

    this->disp->set_cursor(triangle_width + 2, oled_height - (8 + 2));
    this->disp->print(get_val_str(sensor_val));





    this->disp->display();
  }

}

bool RoverInfraredDemo::check_button() {
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
