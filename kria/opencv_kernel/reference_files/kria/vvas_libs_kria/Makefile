# Export all variables to sub-makefiles
export

# Dependencies and libraries
DEPS  = ../include/common.hpp
DEPS += ../include/kernel_config.hpp

INC  = /usr/include

LIBS  = 
LIBS += jansson
LIBS += vvasutil-2.0
LIBS += xrtutil-2.0
LIBS += xrt_core
LIBS += xrt_coreutil

# Compiler and linker
CXX       = g++
CXXFLAGS  = -std=c++17 -fPIC -Wno-narrowing -fpermissive -rdynamic -Wno-narrowing -Wl,--no-undefined -pthread 

LDFLAGS = -shared -Wl,-soname,softkernel.so
LDLIBS  = /usr/lib/aarch64-linux-gnu/libjansson.so 

# Targets
.PHONY: all clean softkernel hardkernel

all: softkernel hardkernel

# Makefiles in subdirs: Make sure the names of the subdirs below are correct!
softkernel:
	make -C softkernel

hardkernel:
	make -C hardkernel
	
clean: 
	make -C softkernel clean
	make -C hardkernel clean
