#!/bin/bash

platform_path="../../kv260_platform/kv260_platform.xpfm"

source /opt/xilinx/2025.1/Vitis/settings64.sh
v++ -t hw --platform $platform_path  --link ../hls/project/conv_top.xo --config link.cfg -o conv_hw.xclbin


