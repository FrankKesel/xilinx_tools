#!/bin/bash

platform_path="../../kv260_platform/kv260_platform.xpfm"

source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -t hw --platform $platform_path  --link ../hls/project/project_work/conv_top.xo --config link.cfg -o conv_hw.xclbin


