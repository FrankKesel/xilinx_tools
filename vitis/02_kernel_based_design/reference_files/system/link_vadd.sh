#!/bin/bash

platform_path="/home/Data/Design/sopc/kria/07_kv260_extensible_platform/kv260_platform/kv260_platform.xpfm"

source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -t hw --platform $platform_path  --link ../hls/project/project_work/krnl_vadd.xo --config link.cfg -o vadd_hw.xclbin


