# Set the project name = name of library
set(LIB_NAME "helper")

# Define minimum required cmake version and
# project name, version and language (C++)
cmake_minimum_required(VERSION 3.18)

# Project name and version
project(LIB_NAME VERSION 1.0)


# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the paths to the include directories
set(XRT_INCLUDE /usr/include/xrt)

# Path to xrt include directories
include_directories(${XRT_INCLUDE})

# Standard include directory
include_directories(include)

# Set all files in directory src as sources (SOURCES Variable)
file(GLOB SOURCES "src/*.cpp")

# Specify the library target
add_library(${LIB_NAME} STATIC ${SOURCES})

# Specify installation rules
install(TARGETS ${LIB_NAME} DESTINATION lib)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h")
