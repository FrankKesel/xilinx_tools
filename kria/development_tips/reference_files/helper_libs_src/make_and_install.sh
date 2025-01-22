#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/home/Data/Design/sopc/kria/06_projects/cpp_libs/helper_libs ..
make
cmake --install .
