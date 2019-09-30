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
#ifndef _MENU_HEADER_
#define _MENU_HEADER_

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

#include <app/RoverDisplay.h>
#include <app/RoverButtons.h>

using namespace std;

class Menu {
public:
  char * name;

  Menu(char *name, RoverButtons* btn, RoverDisplay *disp);

  int get_max_size();

  void set_option(int opt);

  int get_option();

  void inc_option();

  void add_option(char * option, void (*callback)(Menu* menu, RoverButtons* btn, void * closure), void * closure);

  void add_submenu(char * option, Menu *menu);

  void draw();

  Menu * select();

  void update();

  Menu * next();

private:
  vector<char *> options;
  vector<Menu *> submenu;
  vector<void (*)(Menu* menu, RoverButtons* btn, void * closure)> callbacks;
  vector<void *> cookies;
  uint opt = 0;
  RoverButtons *btn;
  RoverDisplay *disp;
};

#endif /* _MENU_HEADER_ */
