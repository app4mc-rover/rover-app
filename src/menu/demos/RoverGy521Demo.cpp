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

#include <demo/RoverGy521Demo.h>

RoverGy521Demo::RoverGy521Demo(RoverGy521 *sensor, RoverDisplay * disp, RoverButtons * btn) {
  this->sensor = sensor;
  this->disp = disp;
  this->btn = btn;
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
    disp->set_cursor(5, 2 + 20 * i);
    disp->print(data_str.c_str());
  }
}

int RoverGy521Demo::run() {
  double sensor_val;
  int int_data[3];
  double double_data[3];
  uint sel = 0;

  this->disp->set_text_size(1);
  this->disp->set_text_color(1);

  while (!check_button()) {

    if (check_next_button()) {
      sel++;
      sel = sel > 2 ? 0 : sel;
    }

    this->disp->clear_display();

    switch (sel) {
      case 0: // Accelerometer
        this->sensor->read_acc_x(int_data[0]);
        this->sensor->read_acc_y(int_data[1]);
        this->sensor->read_acc_z(int_data[2]);
        print_data<int>(this->disp, "acc_", int_data);
        break;
      case 1: // Gyroscope
        this->sensor->read_gyro_x(int_data[0]);
        this->sensor->read_gyro_y(int_data[1]);
        this->sensor->read_gyro_z(int_data[2]);
        print_data<int>(this->disp, "gy_", int_data);
        break;
      case 2: // Angle
        this->sensor->read_angle_x(double_data[0]);
        this->sensor->read_angle_y(double_data[1]);
        this->sensor->read_angle_z(double_data[2]);
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

bool RoverGy521Demo::check_next_button() {
  double state = 1;
  static bool trigered = false;

  this->btn->read(shutdown_button, state);

  if (trigered && state != 0) {
    trigered = false;
    return true;
  }

  if (state == 0) {
    trigered = true;
  }

  return false;
}
