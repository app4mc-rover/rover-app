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

#include <menu/demo/RoverInfraredDemo.h>

static const int oled_width = 128;
static const int oled_height = 64;

RoverInfraredDemo::RoverInfraredDemo(RoverInfraredSensor *r_infrared0,
      RoverInfraredSensor *r_infrared1, 
      RoverInfraredSensor *r_infrared2,
      RoverInfraredSensor *r_infrared3, 
      RoverDisplay * disp, RoverButton * btn) {
  
    this->r_infrared3 = r_infrared3;
    this->r_infrared2 = r_infrared2;
    this->r_infrared1 = r_infrared1;
    this->r_infrared0 = r_infrared0;
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


  this->disp->setTextSize(1);
  this->disp->setTextColor(1);

  while (!check_button()) {
    this->disp->clearDisplay();

    // Draw all triangles
    // Up-left
    this->disp->drawTriangle(0,
                              0,
                              0,
                              triangle_height,
                              triangle_width,
                              0,
                              1);
    // Bottom-left
    this->disp->drawTriangle(0,
                              oled_height,
                              triangle_width,
                              oled_height,
                              0,
                              oled_height - triangle_height,
                              1);

    // Up-right
    this->disp->drawTriangle(oled_width,
                              0,
                              oled_width,
                              triangle_height,
                              oled_width - triangle_width,
                              0,
                              1);
    // Buttom-right
    this->disp->drawTriangle(oled_width,
                              oled_height,
                              oled_width,
                              oled_height - triangle_height,
                              oled_width - triangle_width,
                              oled_height,
                              1);


    // Read Left
    sensor_val = this->r_infrared0->read();
    this->disp->setCursor(80, 2);
    this->disp->print(get_val_str(sensor_val));

    // Read right
    sensor_val = this->r_infrared1->read();
    this->disp->setCursor(80,  oled_height - (8 + 2));
    this->disp->print(get_val_str(sensor_val));


    // Front right
    sensor_val = this->r_infrared2->read();
    this->disp->setCursor(triangle_width + 2, 2);
  	this->disp->print(get_val_str(sensor_val));

    // Front left
    sensor_val = this->r_infrared3->read();
    this->disp->setCursor(triangle_width + 2, oled_height - (8 + 2));
    this->disp->print(get_val_str(sensor_val));

    this->disp->display();
  }

}

bool RoverInfraredDemo::check_button() {
  double state = 1;
  static bool trigered = false;

  state = this->btn->readButton();

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}
