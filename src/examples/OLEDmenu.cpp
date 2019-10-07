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
#include <systemd/sd-event.h>

#include <json-c/json.h>
#include <afb/afb-wsj1.h>
#include <afb/afb-ws-client.h>

#include <app/RoverDriving.h>
#include <app/RoverInfraredSensor.h>
#include <app/RoverGrooveUltrasonicSensor.h>
#include <app/RoverBuzzer.h>
#include <app/RoverUtils.h>
#include <app/RoverDisplay.h>
#include <app/RoverButtons.h>
#include <app/RoverDriving.h>
#include <app/RoverHelloWorld.h>

#include <demo/RoverBuzzerDemo.h>
#include <demo/RoverDrivingDemo.h>
#include <demo/RoverInfraredDemo.h>
#include <demo/RoverGrooveDemo.h>
#include <demo/RoverDht22Demo.h>
#include <demo/RoverGy521Demo.h>
#include <demo/RoverBearingDemo.h>
#include <demo/RoverAccDemo.h>

#include <demo/RoverHelloWorldDemo.h>

#include <icons/bluetooth_logo.h>

#include <Menu.h>
#include <Text.h>
#include <StatusMenu.h>

using namespace std;

// Callback for handling shutdown menu
void shutdown_cb(Menu * menu, RoverButtons* btn, void * closure) {

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
void main_cb(Menu * menu, RoverButtons* btn, void * closure) {
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
void demo_cb(Menu * menu, RoverButtons* btn, void * closure) {
  RoverBuzzerDemo * bzrd = (RoverBuzzerDemo *)closure;
  RoverDrivingDemo * drvd = (RoverDrivingDemo *)closure;
  RoverInfraredDemo * infrd = (RoverInfraredDemo *)closure;
  RoverGrooveDemo * grvsd = (RoverGrooveDemo *)closure;
  RoverDht22Demo * dhtd =  (RoverDht22Demo *)closure;
  RoverGy521Demo * gy521d = (RoverGy521Demo *)closure;
  RoverBearingDemo * beardem = (RoverBearingDemo *)closure;
  RoverAccDemo  * acc_demo = (RoverAccDemo *)closure;
  RoverHelloWorldDemo * hello_demo = (RoverHelloWorldDemo *)closure; 

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
    case 8: // Hello World Test
      hello_demo->run(); 
      break; 
    default:
      break;
  }

  return;
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

  utils.get_number_of_network_interfaces(int_num);

  for (int i = 0; i < int_num; i++) {
    utils.get_interface_info(i, out_interface_name, out_ip_addr, out_hw_add);

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
	int rc = 0;

	/*get port and token from the command arg*/
	char *port = av[1];
	char *token = av[2];
  char uri[500];

  sprintf(uri, "127.0.0.1:%s/api?token=%s", port, token);

  // Create services objects
  RoverDisplay display(uri);
  RoverButtons btn(uri);
  RoverBuzzer bzr(uri);
  RoverDriving drv(uri);
  RoverInfraredSensor inf_red(uri);
  RoverGrooveUltrasonicSensor grv_sen(uri);
  RoverDht22 dht_sen(uri);
  RoverGy521 gy_sen(uri);
  RoverUtils util(uri);
  RoverHmc5883L bear_sen(uri);
  
  RoverHelloWorld hello(uri);
  
  // Create demos objects
  RoverBuzzerDemo bzr_demo(&bzr);
  RoverDrivingDemo drv_demo(&drv, &display, &btn);
  RoverInfraredDemo inf_red_demo(&inf_red, &display, &btn);
  RoverGrooveDemo grv_sen_demo(&grv_sen, &display, &btn);
  RoverDht22Demo dht_sen_demo(&dht_sen, &display, &btn);
  RoverGy521Demo gy_sen_demo(&gy_sen, &display, &btn);
  RoverBearingDemo bearing_demo(&bear_sen, &display, &btn);
  RoverAccDemo acc_demo(&drv, &grv_sen, &display, &btn);

  RoverHelloWorldDemo hello_demo(&hello); 

  StatusMenu status_men(&util, &display, &btn);
  Text info_text(&display, &btn);

  create_info_text(info_text, util);

  // Create the menu objects
  Menu main_menu = Menu("Main", &btn, &display);
  Menu demo_menu = Menu("Demo", &btn, &display);
  Menu shut_menu = Menu("Shutdown", &btn, &display);
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
  demo_menu.add_option("9:Hello", demo_cb, &hello_demo);
  demo_menu.add_submenu("10:Back", &main_menu);

  // Add Shutdown menu options
  shut_menu.add_option("1:Reset", shutdown_cb, NULL);
  shut_menu.add_option("2:Shutdown", shutdown_cb, NULL);
  shut_menu.add_submenu("3:Back", &main_menu);


  while (true) {
    curr_menu->update();
    curr_menu = curr_menu->next();
    curr_menu->draw();
  }

  if (rc) {
    return -1;
  }

}
