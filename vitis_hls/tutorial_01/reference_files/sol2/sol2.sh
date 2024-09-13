#!/bin/bash
source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -c --mode hls --config ./sol2_config.cfg --work_dir sol2_work
