#!/bin/bash

#Run this script as root, from root directory

rm -rf ./build && mkdir -p build && cd build && cmake .. && sudo make && sudo make install
