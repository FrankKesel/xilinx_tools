#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<install-path>/helper_libs ..
make
cmake --install .
