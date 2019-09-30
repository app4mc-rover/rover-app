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

#include <Menu.h>

using namespace std;

Menu::Menu(char *name, RoverButtons* btn, RoverDisplay *disp) {
  this->name = name;
  this->btn = btn;
  this->disp = disp;
}

int Menu::get_max_size() {
  return options.size();
}

void Menu::set_option(int opt) {
  this->opt = opt;
}

int Menu::get_option(){
  return this->opt;
}

void Menu::inc_option() {
  this->opt++;

  if (this->opt >= this->get_max_size()) {
    this->opt = 0;
  }
}

void Menu::add_option(char * option, void (*callback)(Menu* menu, RoverButtons* btn, void * closure), void * closure) {
  options.push_back(option);
  submenu.push_back(NULL);
  callbacks.push_back(callback);
  cookies.push_back(closure);
}

void Menu::add_submenu(char * option, Menu *menu) {
  options.push_back(option);
  submenu.push_back(menu);
  callbacks.push_back(NULL);
  cookies.push_back(NULL);
}

void Menu::draw() {
  int rc = 0;
  bool status;
  int start_opt = 0;
  int end_opt = 3;
  int text_color = 1;
  int bg_color = 0;


  rc |= this->disp->clear_display();

  this->disp->set_text_size(2);
	this->disp->set_text_color(1);

  start_opt = 0;
  if (this->opt >= 3) {
    start_opt = 1 + this->opt - 3;
  }

  for (int i = 0; i < 3; i++) {
    if ((i + start_opt) == this->opt) {
      bg_color = 1;
    } else {
      bg_color = 0;
    }

    this->disp->set_text_color(text_color);
    this->disp->draw_rectangle(0, i*20, 128, 20, bg_color, false);

    this->disp->set_cursor(5, 2 + 20 * i);
  	this->disp->print(options[i + start_opt]);
  }

  this->disp->display();
}

Menu * Menu::select() {
  Menu * ret;
  if (submenu[this->opt] != NULL) {
    ret = submenu[this->opt];
    this->set_option(0);
    return ret;
  }

  if (callbacks[this->opt]) {
    callbacks[this->opt](this, this->btn, cookies[this->opt]);
  }

  return this;
}

void Menu::update() {
  double state = 1;
  static bool trigered = false;

  this->btn->read(shutdown_button, state);

  if (trigered && state != 0) {
    trigered = false;
    this->inc_option();
  }

  if (state == 0) {
    trigered = true;
  }
}

Menu * Menu::next() {
  double state = 1;
  static bool trigered = false;

  this->btn->read(user_button, state);

  if (trigered && state != 0) {
    trigered = false;
    return this->select();
  }

  if (state == 0) {
    trigered = true;
  }

  return this;
}
