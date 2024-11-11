
PLATFORM_FILE := /home/progs/xilinx/kv260_hw_platforms/kv260_platform/kv260_platform.xpfm

TARGET := hw

XOCCFLAGS := --platform $(PLATFORM_FILE) -t $(TARGET) -s -g 

MULTISCALER_FLAGS := --kernel multi_scaler -I. -I./src/hls

.PHONY: clean

all: multi_scaler.xo

multi_scaler.xo: src/v_multi_scaler.cpp src/v_hresampler.cpp src/v_hscaler.cpp src/v_dma.cpp src/v_csc.cpp src/v_vresampler.cpp src/v_vscaler.cpp
	v++ $(XOCCFLAGS) $(MULTISCALER_FLAGS) -c -o xo/$@ $^

clean:
	$(RM) -r xo/* *_x .Xil sd_card* *.xclbin *.ltx *.log *.info packaged_kernel* tmp_kernel* vivado* pfm_sw dpu_conf.vh
	$(RM) *summary* *.str *.hwh
