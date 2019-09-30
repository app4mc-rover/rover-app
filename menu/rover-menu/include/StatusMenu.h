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
#ifndef _STATUS_MENU_HEADER_
#define _STATUS_MENU_HEADER_

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
#include <app/RoverUtils.h>

#include <Menu.h>

using namespace std;

class StatusMenu {
public:

    RoverUtils * util;

    StatusMenu(RoverUtils *util, RoverDisplay * disp, RoverButtons * btn);

    int run();

    bool check_button();

    int screen_sel;

private:

    RoverDisplay * disp;

    RoverButtons * btn;
    Menu * main;

};

#endif /* _STATUS_MENU_HEADER_ */
