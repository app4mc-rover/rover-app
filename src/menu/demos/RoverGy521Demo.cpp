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

#include <menu/demo/RoverGy521Demo.h>

RoverGy521Demo::RoverGy521Demo(RoverGY521 *sensor, 
        RoverDisplay * disp, 
        RoverButton * usrbtn,
        RoverButton * shutdownbtn) {
  this->sensor = sensor;
  this->disp = disp;
  this->usrbtn = usrbtn;
  this->shutdownbtn = shutdownbtn;
}


inline string get_val_str(double val) {
  std::stringstream string_val;

  string_val << std::setprecision(2);
  string_val << std::fixed;

  string_val << val;

  return string_val.str();

}

template <typename T>
string get_string(T data);

template <>
string get_string<int>(int data) {
  return to_string(data);
}

template <>
string get_string<double>(double data) {
  return get_val_str(data);
}

template <typename T>
void print_data(RoverDisplay * disp, const char * prefix, T data[3]) {
  static const char * data_name[3] = {"x", "y", "z"};
  string data_str;

  for (int i = 0; i < 3; i++){
    data_str = string(prefix) + data_name[i] + string(":") + get_string(data[i]);
    disp->setCursor(5, 2 + 20 * i);
    disp->print(data_str.c_str());
  }
}

int RoverGy521Demo::run() {
  double sensor_val;
  int int_data[3];
  double double_data[3];
  uint sel = 0;

  this->disp->setTextSize(1);
  this->disp->setTextColor(1);

  while (!check_button()) {

    if (check_next_button()) {
      sel++;
      sel = sel > 2 ? 0 : sel;
    }

    this->disp->clearDisplay();

    switch (sel) {
      case 0: // Accelerometer
        int_data[0] = this->sensor->getAccelX();
        int_data[1] = this->sensor->getAccelY();
        int_data[2] = this->sensor->getAccelZ();
        print_data<int>(this->disp, "Accel", int_data);
        break;
      case 1: // Gyroscope
        int_data[0] = this->sensor->getGyroX();
        int_data[1] = this->sensor->getGyroY();
        int_data[2] = this->sensor->getGyroZ();
        print_data<int>(this->disp, "gy_", int_data);
        break;
      case 2: // Angle
        double_data[0] = this->sensor->getAngleX();
        double_data[1] = this->sensor->getAngleY();
        double_data[2] = this->sensor->getAngleZ();
        print_data<double>(this->disp, "an_", double_data);
        break;
      default:
        break;
    }

    this->disp->display();
  }

}

bool RoverGy521Demo::check_button() {
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

bool RoverGy521Demo::check_next_button() {
  double state = 1;
  static bool trigered = false;

  state = this->shutdownbtn->readButton();

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}
