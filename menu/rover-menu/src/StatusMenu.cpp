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
#include <chrono>

#include <StatusMenu.h>

#include <icons/bluetooth_logo.h>
#include <icons/kuksa_logo.h>

StatusMenu::StatusMenu(RoverUtils *util, RoverDisplay * disp, RoverButtons * btn) {
  this->util = util;
  this->disp = disp;
  this->btn = btn;
}

int StatusMenu::run() {
  bool status;
  static auto prev = chrono::system_clock::now();
  static auto curr = chrono::system_clock::now();
  screen_sel = 0;

  this->disp->set_text_size(2);
  this->disp->set_text_color(1);

  while (!check_button()) {
    this->disp->clear_display();

    this->disp->set_text_size(2);
    this->disp->set_text_color(1);

    switch (screen_sel) {
      case 0:
        this->disp->set_cursor(45, 10);
        this->disp->print("WLAN:");
        util->get_wlan_status(status);
        break;
      case 1:
        this->disp->set_cursor(48, 10);
        this->disp->print("ETH:");
        util->get_ethernet_status(status);
        break;
      case 2:
        this->disp->set_cursor(12, 10);
        this->disp->print("INTERNET:");
        util->get_internet_status(status);
        break;
      case 3:
        this->disp->set_cursor(8, 10);
        this->disp->print("BLUETOOTH:");
        util->get_bluetooth_status(status);
        break;
      default:
        screen_sel = 0;
        this->disp->set_cursor(45, 10);
        this->disp->print("WLAN:");
        util->get_wlan_status(status);
    }


    this->disp->set_text_size(3);
    this->disp->set_text_color(1);

    if (status) {
      this->disp->set_cursor(50, 32);
      this->disp->print("ON");
    } else {
      this->disp->set_cursor(43, 32);
      this->disp->print("OFF");
    }

    curr = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = curr - prev;

    if (elapsed_seconds.count() > 2) {
      screen_sel++;
      prev = curr;
    }

    this->disp->display();
  }

}

bool StatusMenu::check_button() {
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
