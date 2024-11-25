#!/bin/bash
# Author: FK, 25.11.24
# Test RTP transmission to Laptop on port 50000 (check IP below)
# Note: Start GST script on Laptop

#Set width and height
width=640
height=480
#Set IP address of receiving computer
ip_address=192.168.178.62

#Launch Gstreamer
gst-launch-1.0 videotestsrc ! video/x-raw, width=$width, height=$height, format=RGB, framerate=30/1 !  rtpvrawpay ! udpsink host=$ip_address port="50000"
