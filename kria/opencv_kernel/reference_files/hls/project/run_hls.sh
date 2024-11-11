#!/bin/bash


source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -c --mode hls --config ./project.cfg --work_dir project_work
