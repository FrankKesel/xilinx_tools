#!/bin/bash
source /opt/xilinx/2026.1/Vitis/settings64.sh
v++ -c --mode hls --config ./sol2/sol2_config.cfg --work_dir sol2
