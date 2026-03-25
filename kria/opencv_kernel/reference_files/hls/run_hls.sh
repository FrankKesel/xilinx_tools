#!/bin/bash


source /opt/xilinx/2025.1/Vitis/settings64.sh
v++ -c --mode hls --config ./project/project.cfg --work_dir project
