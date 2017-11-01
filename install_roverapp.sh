#!/bin/bash

#Run this script as root from root directory

rm -rf ./build && mkdir -p build && cd build && cmake .. && make && sudo make install
