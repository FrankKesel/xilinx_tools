#!/bin/bash

platform_path="../../kv260_platform/kv260_platform.xpfm"

source /opt/xilinx/2025.1/Vitis/settings64.sh
v++ -t hw --platform $platform_path  --link ../hls/project/krnl_vadd.xo --config link.cfg -o vadd_hw.xclbin


