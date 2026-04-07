#!/bin/bash

set -e
mkdir -p build
cd build
cmake -DPICO_BOARD=pico2 ..
make
cd ..
