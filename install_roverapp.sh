#!/bin/bash

rm -rf ./build && mkdir -p build && cd build && cmake .. && sudo make && sudo make install
