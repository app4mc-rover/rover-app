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
#ifndef _TEXT_HEADER_
#define _TEXT_HEADER_

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

class Text {
public:
  Text(RoverDisplay *disp, RoverButtons* btn);

  void add_text(string text);

  void add_separator();

  void add_newline();

  void print();

private:
  vector<string> fulltext;
  uint pos = 0;
  RoverButtons *btn;
  RoverDisplay *disp;
  uint char_size = 1;
  uint num_chars_line = 128/6;
  uint line_height = 10;
  bool running;

  void scroll_down();

  void draw();

  void update();

  int wrap_long_word(string word);
};

#endif /* _TEXT_HEADER_ */
