# Set the project name = name of executable
set(PROJ_NAME conv_single)

# Define minimum required cmake version and
# project name, version and language (C++)
cmake_minimum_required(VERSION 3.18)
project(${PROJ_NAME} VERSION 1.0 LANGUAGES CXX)

# Define SYSROOT for cross compile
set(CMAKE_SYSROOT <your-sysroot-path>/iot-limerick-kria/aarch64-xilinx-linux)

# Define path to helpers library (must be cross compiled for aarch64!)
set(HELPER_INCLUDE <path-to-helper-libs>/helper_libs/include)
set(HELPER_LIBS <path-to-helper-libs>/helper_libs/lib/libhelper.a)

# Search OpenCV package and include dirs
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Set the paths to the include directories
set(XRT_INCLUDE ${CMAKE_SYSROOT}/usr/include/xrt)
# Set necessary project libraries (for linking)
set(XRT_LIBS xrt_coreutil)

# Path to library include directories
include_directories(${HELPER_INCLUDE})
# Path to xrt include directories
include_directories(${XRT_INCLUDE})
# Standard include directory
include_directories(include)

# Set all files in directory src as sources (SOURCES Variable)
file(GLOB SOURCES "src/*.cpp")

# Specify executable/target name and source dependencies
add_executable(${PROJ_NAME} ${SOURCES})

# Specify compile options for the target
target_compile_options(${PROJ_NAME} PRIVATE -std=c++17 -O3)

# Link the target against library
target_link_libraries(${PROJ_NAME} ${XRT_LIBS} ${OpenCV_LIBS} ${HELPER_LIBS})
