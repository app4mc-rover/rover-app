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

#include <demo/RoverDrivingDemo.h>

#include <Menu.h>

void menu_cb(Menu * menu, RoverButtons* btn, void * closure) {

  RoverDrivingDemo * drv_demo = (RoverDrivingDemo *)closure;

  switch (menu->get_option()) {
    case 0: // Increment Speed
      drv_demo->curr_speed += 10;

      if (drv_demo->curr_speed > 480) {
        drv_demo->curr_speed = 480;
      }
      drv_demo->drv->setspeed(drv_demo->curr_speed);
      break;
    case 1: // Decrement speed
      drv_demo->curr_speed -= 10;
      drv_demo->drv->setspeed(drv_demo->curr_speed);
      break;
    case 2: // Go Forward
      drv_demo->drv->goforward();
      break;
    case 3: // Go Backward
      drv_demo->drv->gobackward();
      break;
    case 4: // Turn left
      drv_demo->drv->turnleft();
      break;
    case 5: // Turn right
      drv_demo->drv->turnright();
      break;
    case 6: // Stop
      drv_demo->drv->stop();
      break;
    case 7: // End demo
      drv_demo->running = false;
      break;
    default:
      drv_demo->drv->stop();
  }

  return;
}

RoverDrivingDemo::RoverDrivingDemo(RoverDriving *drv, RoverDisplay * disp, RoverButtons * btn) : curr_speed(0), running(true) {
  this->drv = drv;
  this->btn = btn;
  this->main = new Menu("DrivingDemo", btn, disp);

  this->main->add_option("1:Speed++", menu_cb, this);
  this->main->add_option("2:Speed--", menu_cb, this);
  this->main->add_option("3:Forward", menu_cb, this);
  this->main->add_option("4:Backward", menu_cb, this);
  this->main->add_option("5:Left", menu_cb, this);
  this->main->add_option("6:Right", menu_cb, this);
  this->main->add_option("7:Stop", menu_cb, this);
  this->main->add_option("8:End", menu_cb, this);
}

int RoverDrivingDemo::run() {

  this->drv->setspeed(480);

  while(this->running) {
    this->main->update();
    this->main->next();
    this->main->draw();
  }

}
