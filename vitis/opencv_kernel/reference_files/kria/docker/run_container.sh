#!/bin/bash

docker run \
--env="DISPLAY" \
--env="XDG_SESSION_TYPE" \
-h "kria-docker" \
--net=host \
--privileged \
--name "videostream_container" \
--volume="$HOME/.Xauthority:/root/.Xauthority:rw" \
-v /tmp:/tmp \
-v /dev:/dev \
-v /sys:/sys \
-v /etc/vart.conf:/etc/vart.conf \
-v /lib/firmware/xilinx:/lib/firmware/xilinx \
-v /run:/run \
-v /home/ubuntu/projects:/home/ubuntu/projects \
-it videostream_img:latest bash
