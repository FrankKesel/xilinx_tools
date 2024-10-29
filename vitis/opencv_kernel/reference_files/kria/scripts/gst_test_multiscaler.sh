#!/bin/bash
#Author: FK, 7.2.24

# ------- gst-launch script for gstreamer pipeline -----------------------------
# Convert usb cam video stream to gray scale, and stream
# video over RTP H264 encoded to laptop
# Note: Make sure correct HW platform is loaded (VCU, multi_scaler)

# Define location of FPGA binary and JSON-file for IP kernel on Kria
xclbin_loc="/lib/firmware/xilinx/convolution/binary_container_1.bin"
# Define IP address of the host computer
ip_address=192.168.178.62
# Define frame size and framerate
width=1280
height=720
#width=1920
#height=1080
# Framerate: 30 is max.
framerate=30/1

# Pipeline description:
# v4l2src: USB cam source
# video/x-raw: Caps filter, define video format and framerate
# vvas_xabrscaler: Multi_scaler, convert color to gray
# video/x-raw: Caps filter, define video format and framerate, 8 Bit gray
# omxh264enc: h264 encoder
# rtph264pay: Convert h264 data to RTP packets
# udpsink: Send data as UDP packets to the destination host/port
gst-launch-1.0 \
v4l2src device=/dev/video0 io-mode=mmap ! \
video/x-raw, width=$width, height=$height, format=NV12, framerate=$framerate ! \
vvas_xabrscaler xclbin-location=$xclbin_loc kernel-name=multi_scaler:{multi_scaler_1} scale-mode=2 ! \
video/x-raw, width=$width, height=$height, format=GRAY8, framerate=$framerate ! \
perf ! \
omxh264enc target-bitrate=3000 periodicity-idr=250 ! \
rtph264pay ! \
udpsink host=$ip_address port=50000 sync=false -v
