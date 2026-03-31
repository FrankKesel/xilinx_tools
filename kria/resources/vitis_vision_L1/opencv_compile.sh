# Compiling OpenCV for Vitis Vision Libs
# 2025-03-30 MG

XILINX_VERSION=2025.1
XILINX_GCC_VERSION=8.3.0
OPENCV_VERSION=4.4.0
DOCKER_IMAGE=ubuntu:noble
OPENCV_INSTALL_PATH=/opt/opencv${OPENCV_VERSION}_vitis${XILINX_VERSION}_gcc${XILINX_GCC_VERSION}_docker
MAKE_JOBS=20

XILINX_HOME=/opt/xilinx/${XILINX_VERSION}/Vitis
TMP_SCRIPT=/tmp/docker_script_${RANDOM}.sh


cat << EOF > ${TMP_SCRIPT}
if [ -x ${XILINX_HOME}/bin/vitis ]; then
    apt update && apt -y upgrade
    apt -y install g++ make cmake build-essential git gcc-multilib libprotobuf-dev
    mkdir ${OPENCV_INSTALL_PATH}
    mkdir /opencv
    cd /opencv
    mkdir build
    git clone https://github.com/opencv/opencv.git
    git clone https://github.com/opencv/opencv_contrib.git
    cd /opencv/opencv/ && git checkout ${OPENCV_VERSION}
    cd /opencv/opencv_contrib/ && git checkout ${OPENCV_VERSION}
    cd /opencv/build/
    export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_PATH} -D CMAKE_CXX_COMPILER=${XILINX_HOME}/tps/lnx64/gcc-${XILINX_GCC_VERSION}/bin/g++ -D OPENCV_EXTRA_MODULES_PATH=/opencv/opencv_contrib/modules/ -D WITH_V4L=ON -DBUILD_TESTS=OFF -DBUILD_ZLIB=ON -DBUILD_JPEG=ON -DWITH_JPEG=ON -DWITH_PNG=ON -DBUILD_EXAMPLES=OFF -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_OPENEXR=OFF -DBUILD_OPENEXR=OFF ../opencv
    make all -j${MAKE_JOBS}
    make install
else
    echo "ERROR: Missing Vitis installation in ${XILINX_HOME}"
fi
EOF

if [ -x /usr/bin/docker ]; then
    docker run --rm -it -v /opt:/opt -v ${TMP_SCRIPT}:${TMP_SCRIPT} ${DOCKER_IMAGE} /bin/bash ${TMP_SCRIPT}
else
    echo "ERROR: Missing docker"
fi

rm ${TMP_SCRIPT}
