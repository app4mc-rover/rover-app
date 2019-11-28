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
#include <sstream>
#include <array>
#include <memory>
//#include <systemd/sd-event.h>

//#include <json-c/json.h>
//#include <afb/afb-wsj1.h>
//#include <afb/afb-ws-client.h>

//Basis Include
#include <roverapi/rover_api.hpp>

#include <roverapi/rover_driving.hpp>
#include <roverapi/rover_infraredsensor.hpp>
#include <roverapi/rover_grooveultrasonic.hpp>
#include <roverapi/rover_hcsr04.hpp>
#include <roverapi/rover_buzzer.hpp>
#include <roverapi/rover_utils.hpp>
#include <roverapi/rover_display.hpp>
#include <roverapi/rover_button.hpp>
#include <roverapi/rover_driving.hpp>

#include <menu/demo/RoverBuzzerDemo.h>
#include <menu/demo/RoverDrivingDemo.h>
#include <menu/demo/RoverInfraredDemo.h>
#include <menu/demo/RoverGrooveDemo.h>
#include <menu/demo/RoverDht22Demo.h>
#include <menu/demo/RoverGy521Demo.h>
#include <menu/demo/RoverBearingDemo.h>
#include <menu/demo/RoverAccDemo.h>

#include <menu/icons/bluetooth_logo.h>

#include <menu/Menu.h>
#include <menu/Text.h>
#include <menu/StatusMenu.h>

using namespace std;
using namespace rover;

// Callback for handling shutdown menu
void shutdown_cb(Menu * menu, RoverButton* usrbtn, RoverButton* shutdownbtn, void * closure) {

  switch (menu->get_option()) {
    case 0: // Reset
      system("shutdown -r now");
      break;
    case 1: // Shutdown
      system("shutdown -h now");
      break;
    default:
      break;
  }

  return;
}

// Callback for handling Main menu
void main_cb(Menu * menu, RoverButton* usrbtn, RoverButton* shutdownbtn, void * closure) {
  StatusMenu * status = (StatusMenu *)closure;
  Text * text = (Text *)closure;

  switch (menu->get_option()) {
    case 1:
      status->run();
      break;
    case 2:
      text->print();
      break;
    default:
      break;
  }

  return;
}

// Callback for handling demo menu
void demo_cb(Menu * menu, RoverButton* usrbtn, RoverButton *shutdownbtn, void * closure) {
  RoverBuzzerDemo * bzrd = (RoverBuzzerDemo *)closure;
  RoverDrivingDemo * drvd = (RoverDrivingDemo *)closure;
  RoverInfraredDemo * infrd = (RoverInfraredDemo *)closure;
  RoverGrooveDemo * grvsd = (RoverGrooveDemo *)closure;
  RoverDht22Demo * dhtd =  (RoverDht22Demo *)closure;
  RoverGy521Demo * gy521d = (RoverGy521Demo *)closure;
  RoverBearingDemo * beardem = (RoverBearingDemo *)closure;
  RoverAccDemo  * acc_demo = (RoverAccDemo *)closure;

  switch (menu->get_option()) {
    case 0: // Buzzer
      bzrd->run();
      break;
    case 1: // Driving
      drvd->run();
      break;
    case 2: // Infrared
      infrd->run();
      break;
    case 3: // Groove
      grvsd->run();
      break;
    case 4: // DHT22
      dhtd->run();
      break;
    case 5: // GY521
      gy521d->run();
      break;
    case 6: // Bearing
      beardem->run();
      break;
    case 7: // ACC
      acc_demo->run();
      break;
    default:
      break;
  }

  return;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
        result += buffer.data();
    }
    return result;
}


void get_interface_data(string int_name, std::string &ip, std::string  &mac) {
    string data_ex;
    int find_res = 0;

    string cmd = string("ifconfig ") + int_name;

    stringstream interfaces(exec(cmd.c_str()));

    while (interfaces >> data_ex) {
        if (!data_ex.compare("inet")) {
            interfaces >> data_ex;
            find_res = data_ex.find("addr:");

            if (find_res != string::npos) {
                    data_ex = data_ex.substr(5);
            }
            ip = data_ex.c_str();
        }

        if (!data_ex.compare("ether")) {
            interfaces >> data_ex;
            mac = data_ex.c_str();
        }

        if (!data_ex.compare("HWaddr")) {
            interfaces >> data_ex;
            mac = data_ex.c_str();
        }
    }
}


void get_interfaces(vector<string> &interfaces) {
  string int_name;
  stringstream cmd_out(exec("ifconfig -a | grep '^[a-z].*' | awk '{ print $1}'"));

  while (cmd_out >> int_name) {
      interfaces.push_back(int_name);
    }

}

int get_interface_info(const int in_interface_idx,
        std::string &out_interface_name,
        std::string &out_ip_addr,
        std::string &out_hw_addr) {

    vector<string> interfaces;
    get_interfaces(interfaces);

    if (in_interface_idx >= interfaces.size()) {
            return -1;
    }

    out_interface_name = interfaces[in_interface_idx];

    get_interface_data(interfaces[in_interface_idx], out_ip_addr, out_hw_addr);

    return 0;
}

int get_number_of_network_interfaces(int &out_num_interface) {
    vector<string> interfaces;
    get_interfaces(interfaces);
    
    out_num_interface = interfaces.size();
    return 0;
}

void create_info_text(Text &text, RoverUtils &utils) {
  int int_num = 0;
  std::string out_interface_name;
  std::string out_ip_addr;
  std::string out_hw_add;
  string info;

  text.add_text("====================");
  text.add_text("NETWORK");
  text.add_text("====================");

  get_number_of_network_interfaces(int_num);

  for (int i = 0; i < int_num; i++) {
    get_interface_info(i, out_interface_name, out_ip_addr, out_hw_add);

    if (!out_interface_name.compare("lo")) {
      continue;
    }

    info = std::string("Interface: ") + out_interface_name;
    text.add_text(info);
    text.add_text("--------------------");
    info = std::string("IP: ") + out_ip_addr;
    text.add_text(info);
    text.add_text("--------------------");
    info = std::string("HW: ") + out_hw_add;
    text.add_text(info);
    text.add_text("--------------------");
  }
}

/* entry function */
int main(int ac, char **av, char **env)
{
    RoverBase r_base = RoverBase();
    r_base.initialize();

    // Use the OLED display on the rover
    RoverDisplay r_display = RoverDisplay();
    r_display.initialize();
  
    // Instantiate buzzer and button
    RoverBuzzer r_buzzer = RoverBuzzer();
    RoverButton user_button = RoverButton(USER_BUTTON); //right one
    RoverButton shutdown_button = RoverButton(SHUTDOWN_BUTTON); //left one
    r_buzzer.initialize();
    user_button.initialize();
    shutdown_button.initialize();
 
    // Driving with rover
    RoverDriving r_driving = RoverDriving();
    r_driving.initialize();
    r_driving.setSpeed(HIGHEST_SPEED);

    // Set up ultrasonic sensors
    RoverHCSR04 r_front = RoverHCSR04(ROVER_FRONT);
    RoverHCSR04 r_rear = RoverHCSR04(ROVER_REAR);
    r_front.initialize();
    r_rear.initialize();

    // InfraredSensor
    RoverInfraredSensor r_infrared0 = RoverInfraredSensor(ROVER_REAR_RIGHT);
    RoverInfraredSensor r_infrared1 = RoverInfraredSensor(ROVER_REAR_LEFT);
    RoverInfraredSensor r_infrared2 = RoverInfraredSensor(ROVER_FRONT_RIGHT);
    RoverInfraredSensor r_infrared3 = RoverInfraredSensor(ROVER_FRONT_LEFT);
    r_infrared0.initialize();
    r_infrared1.initialize();
    r_infrared2.initialize();
    r_infrared3.initialize();

    // Set up HMC5883L or QMC5883L compass sensor
    RoverHMC5883L r_hmc = RoverHMC5883L();
    r_hmc.initialize();
    r_hmc.calibrate();

    // Set up GY521 accelerometer
    RoverGY521 r_accel = RoverGY521();
    r_accel.initialize();

    //  DHT22 temperature and humidity
    RoverDHT22 r_dht22 = RoverDHT22();
    r_dht22.initialize();


    // Instantiate a RoverUtils object to access member functions that deals with status and performance tasks
    RoverUtils r_utils = RoverUtils(); //RoverUtils does not need to be initialized
 
    r_base.sleep(100);
 
  // Create demos objects
  RoverBuzzerDemo bzr_demo(&r_buzzer);
  RoverDrivingDemo drv_demo(&r_driving, &r_display, &user_button, &shutdown_button);
  RoverInfraredDemo inf_red_demo(
          &r_infrared0, &r_infrared1, &r_infrared2, &r_infrared3 , 
          &r_display, &user_button);
  RoverGrooveDemo grv_sen_demo(&r_front, &r_rear, &r_display, &user_button);
  RoverDht22Demo dht_sen_demo(&r_dht22, &r_display, &user_button);
  RoverGy521Demo gy_sen_demo(&r_accel, &r_display, &user_button, &shutdown_button);
  RoverBearingDemo bearing_demo(&r_hmc, &r_display, &user_button);
  RoverAccDemo acc_demo(&r_driving, &r_front, &r_rear, &r_display, &user_button);

  StatusMenu status_men(&r_utils, &r_display, &user_button);
  Text info_text(&r_display, &user_button, &shutdown_button);

  create_info_text(info_text, r_utils);

  // Create the menu objects
  Menu main_menu = Menu("Main", &user_button, &shutdown_button, &r_display);
  Menu demo_menu = Menu("Demo", &user_button, &shutdown_button, &r_display);
  Menu shut_menu = Menu("Shutdown", &user_button, &shutdown_button, &r_display);
  Menu * curr_menu = &main_menu;

  // Add Main Menu options
  main_menu.add_submenu("1:Demo", &demo_menu);
  main_menu.add_option("2:Status", main_cb, &status_men);
  main_menu.add_option("3:Info", main_cb, &info_text);
  main_menu.add_submenu("4:Shutdown", &shut_menu);

  // Add Demo Menu options
  demo_menu.add_option("1:Buzzer", demo_cb, &bzr_demo);
  demo_menu.add_option("2:Driving", demo_cb, &drv_demo);
  demo_menu.add_option("3:Infrared", demo_cb, &inf_red_demo);
  demo_menu.add_option("4:Groove", demo_cb, &grv_sen_demo);
  demo_menu.add_option("5:DHT22", demo_cb, &dht_sen_demo);
  demo_menu.add_option("6:GY521", demo_cb, &gy_sen_demo);
  demo_menu.add_option("7:Bearing", demo_cb, &bearing_demo);
  demo_menu.add_option("8:ACC", demo_cb, &acc_demo);
    // Test service
  demo_menu.add_submenu("9:Back", &main_menu);

  // Add Shutdown menu options
  shut_menu.add_option("1:Reset", shutdown_cb, NULL);
  shut_menu.add_option("2:Shutdown", shutdown_cb, NULL);
  shut_menu.add_submenu("3:Back", &main_menu);


  while (true) {
    curr_menu->update();
    curr_menu = curr_menu->next();
    curr_menu->draw();
  }

}
