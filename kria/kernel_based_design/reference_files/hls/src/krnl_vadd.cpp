/**
 * @file krn_vadd.cpp
 * @author Frank Kesel
 * @date 29 Nov 2022
 * @version 1.0
 * @brief Vector addition HLS demo
 * @details
 */

#include <stdint.h>
#include <hls_stream.h>
#include "krnl_vadd.h"


/**
 * @brief Input mover
 * @param in : input vector
 * @param in_stream : input stream
 * @param size: size of vectors
 * @details Copies data from input to local stream
**/
void load_input(uint32_t* in,
				hls::stream<uint32_t>& in_stream, int size) {
	mem_rd: for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        in_stream << in[i];
    }
}

/**
 * @brief Compute unit
 * @param in1_stream : input stream
 * @param in2_stream : input stream
 * @param out_stream : output stream
 * @param size: size of vectors
 * @details Adds input streams to output stream
**/
void compute_add(hls::stream<uint32_t>& in1_stream,
                 hls::stream<uint32_t>& in2_stream,
                 hls::stream<uint32_t>& out_stream,
                 int size) {
	execute: for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out_stream << (in1_stream.read() + in2_stream.read());
    }
}

/**
 * @brief Output mover
 * @param out : output vector
 * @param out_stream : output stream
 * @param size: size of vectors
 * @details Copies data from local stream to output
**/
void store_result(uint32_t* out,
				hls::stream<uint32_t>& out_stream, int size) {
	mem_wr: for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out[i] = out_stream.read();
    }
}


/**
 * @brief Toplevel krnl_vadd IP core
 * @param in1 : Input vector 1
 * @param in2 : Input vector 2
 * @param out : Output vector
 * @param size: size of vectors
 * @details
*/
void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size) {
// Define interfaces
// AXI Lite Slave interface
#pragma HLS INTERFACE mode=s_axilite port=size
#pragma HLS INTERFACE mode=s_axilite port=return
// AXI Master interfaces: There are 2 bus interfaces
// for the inputs to achieve higher throughput
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=c_size port=in1
#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=c_size port=in2
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=c_size port=out

	//Define local stream buffers
    static hls::stream<uint32_t> in1_stream("input_stream_1");
    static hls::stream<uint32_t> in2_stream("input_stream_2");
    static hls::stream<uint32_t> out_stream("output_stream");

// dataflow pragma instruct compiler to run following three APIs in parallel
#pragma HLS dataflow

    load_input(in1, in1_stream, size);
    load_input(in2, in2_stream, size);
    compute_add(in1_stream, in2_stream, out_stream, size);
    store_result(out, out_stream, size);
}

