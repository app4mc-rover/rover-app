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

#include <menu/demo/RoverDrivingDemo.h>

#include <menu/Menu.h>

void menu_cb(Menu * menu, RoverButton* usrbtn, RoverButton * shutdownbtn, void * closure) {

  RoverDrivingDemo * drv_demo = (RoverDrivingDemo *)closure;

  switch (menu->get_option()) {
    case 0: // Increment Speed
      drv_demo->curr_speed += 10;

      if (drv_demo->curr_speed > HIGHEST_SPEED) {
        drv_demo->curr_speed = HIGHEST_SPEED;
      }
      drv_demo->drv->setSpeed(drv_demo->curr_speed);
      break;
    case 1: // Decrement speed
      drv_demo->curr_speed -= 10;
      drv_demo->drv->setSpeed(drv_demo->curr_speed);
      break;
    case 2: // Go Forward
      drv_demo->drv->goForward();
      break;
    case 3: // Go Backward
      drv_demo->drv->goBackward();
      break;
    case 4: // Turn left
      drv_demo->drv->turnLeft();
      break;
    case 5: // Turn right
      drv_demo->drv->turnRight();
      break;
    case 6: // Stop
      drv_demo->drv->stopRover();
      break;
    case 7: // End demo
      drv_demo->running = false;
      break;
    default:
      drv_demo->drv->stopRover();
  }

  return;
}

RoverDrivingDemo::RoverDrivingDemo(RoverDriving *drv, RoverDisplay * disp, RoverButton * usrbtn, RoverButton* shutdownbtn) : curr_speed(0), running(true) {
  this->drv = drv;
  this->usrbtn= usrbtn;
  this->shutdownbtn = shutdownbtn;
  this->main = new Menu("DrivingDemo", usrbtn, shutdownbtn, disp);

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

  this->drv->setSpeed(480);

  while(this->running) {
    this->main->update();
    this->main->next();
    this->main->draw();
  }

}
