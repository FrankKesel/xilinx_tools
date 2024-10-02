#!/bin/bash
source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -c --mode hls --config ./sol1_config.cfg --work_dir sol1_work
