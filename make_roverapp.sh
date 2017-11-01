#!/bin/bash

#Run this script from root directory

rm -rf ./build && mkdir -p build && cd build && cmake .. && make 
