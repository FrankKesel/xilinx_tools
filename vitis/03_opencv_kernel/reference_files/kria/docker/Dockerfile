FROM xilinx/kria-runtime:2022.1

LABEL Description="Docker image for development and running videostream applications" Vendor="hspf" Version="1.0"
ARG BUILD_DATE
ARG BUILD_VERSION
LABEL org.label-schema.schema-version="1.0"
LABEL org.label-schema.build-date=$BUILD_DATE
LABEL org.label-schema.version=$BUILD_VERSION

# to avoid configuring tzdata for opencv
ARG DEBIAN_FRONTEND=noninteractive

# Add pgRouting launchpad repository
RUN \
apt update && apt install --yes --no-install-recommends \
    git \
    cmake \
    build-essential \ 
    libasound-dev \
    libgstreamer1.0-dev \
    libopencv-dev \
    libgoogle-glog-dev \
    sudo \
    libprotobuf-dev \
    bash \
    libjansson-dev \
    vim \
    libgstrtspserver-1.0-dev \
    libgstreamer1.0-dev \
    libboost-serialization1.74.0 \
    libboost-serialization1.74-dev \
    libzmq3-dev \
    libgstreamer-plugins-good1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-bad1.0-dev \
    libopencv-dev \
    libxilinx-vcu-omx-dev ;

RUN \
    apt install --yes graphviz ; \
    pip install pydot ; 
