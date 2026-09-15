#!/bin/bash
source /opt/xilinx/2026.1/Vitis/settings64.sh
v++ -c --mode hls --config ./sol1/sol1_config.cfg --work_dir sol1
