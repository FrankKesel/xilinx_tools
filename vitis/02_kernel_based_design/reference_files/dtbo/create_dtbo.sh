#!/bin/bash
echo "Create device tree dtbo"

source /opt/xilinx/Vitis/2024.1/settings64.sh
xsct dts.tcl
dtc -@ -O dtb -o pl.dtbo devicetree/devicetree/kv260_platform/psu_cortexa53_0/device_tree_domain/bsp/pl.dtsi
