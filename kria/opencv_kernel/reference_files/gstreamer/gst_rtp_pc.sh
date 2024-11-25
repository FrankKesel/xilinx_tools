#!/bin/bash
#Author: FK, 25.11.24
# Receive a unencoded video stream via RTP on port 50000

#Set width and height
width=640
height=480

#Launch Gstreamer, listen on port 5000
gst-launch-1.0 udpsrc port=50000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)RGB, width=(string)$width, height=(string)$height, payload=(int)96" ! rtpvrawdepay ! videoconvert ! queue ! ximagesink -v
