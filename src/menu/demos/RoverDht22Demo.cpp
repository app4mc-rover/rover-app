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
#include <string>

#include <menu/demo/RoverDht22Demo.h>

using namespace rover;


RoverDht22Demo::RoverDht22Demo(RoverDHT22 *sensor, RoverDisplay * disp, RoverButton * btn) {
  this->sensor = sensor;
  this->disp = disp;
  this->btn = btn;
}

int RoverDht22Demo::run() {
  double sensor_val = 1;

  this->disp->setTextSize(2);
  this->disp->setTextColor(1);

  while (!check_button()) {
    this->disp->clearDisplay();

    // Temperature
    sensor_val = this->sensor->readTemperature();
    this->disp->setCursor(30, 30);
    this->disp->print(to_string(sensor_val).c_str());


    // Humidity
    // this->sensor->read_humidity(sensor_val);
    this->disp->setCursor(90, 30);
    this->disp->print(to_string(sensor_val).c_str());


    this->disp->display();
  }

}

bool RoverDht22Demo::check_button() {
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
