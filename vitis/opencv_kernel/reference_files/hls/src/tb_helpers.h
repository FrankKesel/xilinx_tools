/**
 * @file tb_helpers.h
 * @author Frank Kesel
 * @date 17 Jan 2023
 * @version 1.0
 * @brief HLS Testbench helper functions
 * @details
 */

#ifndef TB_HELPERS_H_
#define TB_HELPERS_H_

#include "project.h"

/**
 * @brief Read image data from file
 * @details Reads 8 bit grayscale image data from a file
 * 	data is expected in binary format
 * @param file_name: file name
 * @param image: array to store image data
 * @tparam HEIGHT: height of image
 * @tparam WIDTH: width of image
 * @tparam T: data type of image (only 8 Bit allowed)
*/
template <unsigned int HEIGHT, unsigned int WIDTH, typename T>
int read_image(const char *fileName, T image[HEIGHT][WIDTH]){
	//Open the file in binary mode
    ifstream infile(fileName, ios::in | ios::binary);
    if (!infile.is_open()){
        cout << "ERROR: File " << fileName << " could not be opened!" <<endl;
        return 1;
    }

    unsigned char buffer[WIDTH]; //Line buffer
    //Read the data line by line from the file
    for(unsigned int i = 0; i < HEIGHT; ++i){
    	infile.read((char *)buffer, WIDTH); //Read one line from the file
        for(unsigned int j = 0; j < WIDTH; ++j){
            image[i][j] = buffer[j]; //Copy buffer to the matrix
        }
    }

    infile.close();
    return 0;
}

/**
 * @brief Read filter data from file
 * @details Reads 16 bit filter data from a file
 * 	and stores it in an array, data is expected in binary format
 * 	in little endian format
 * @param file_name: file name
 * @param filter: array to store image data
 * @tparam SIZE: filter size
*/
template <unsigned int SIZE>
int read_filter(const char *fileName, short int filter[SIZE]){
	//Open the file in binary mode
    ifstream infile(fileName, ios::in | ios::binary);
    if (!infile.is_open()){
        cout << "ERROR: File " << fileName << " could not be opened!" <<endl;
        return 1;
    }

    short int var; //Buffer
    //Read the data from the file, 2 bytes are combined to a 16 bit integer
    for(unsigned int i = 0; i < SIZE; ++i){
    	infile.read((char *)&var, 2); //Get one data
    	filter[i] = var; //Copy to filter array
    }

    infile.close();
    return 0;
}

/**
 * @brief Compare two images
 * @details
 * @param res: first array
 * @param ref: second array
 * @tparam HEIGHT: height of image
 * @tparam WIDTH: width of image
 * @tparam T: data type
*/
template <unsigned int HEIGHT, unsigned int WIDTH, typename T>
int compare_image(const T res[HEIGHT][WIDTH],
				  const T ref[HEIGHT][WIDTH]){
    int failed = 0;
    for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){
            if (ref[i][j] != res[i][j])
                failed++;
        }
    }
    return failed;
}


/**
 * @brief Write image data to file
 * @details Writes data of a matrix to a text file
 * @param file_name: file name
 * @param image: array where image data is stored
 * @tparam HEIGHT: height of image
 * @tparam WIDTH: width of image
 * @tparam T: data type (only an 8 bit data type allowed)
*/
template <unsigned int HEIGHT, unsigned int WIDTH, typename T>
int write_image(const char *fileName, const T image[HEIGHT][WIDTH]){
    ofstream outfile(fileName, ios::out);
    if (!outfile.is_open()){
        cout << "ERROR: File " << fileName << " could not be opened!\n";
        return 1;
    }

    for (unsigned int i = 0; i < HEIGHT; ++i){
        for (unsigned int j = 0; j < WIDTH; ++j){
        	//Write data decimal with a field width of 3
            outfile << setw(3) << dec << image[i][j] << " ";
        }
        outfile << endl; //Line end
    }
    outfile.close();
    return 0;
}

/**
 * @brief Write image data to file in binary format
 * @details Writes data of a matrix to a text file
 * @param file_name: file name
 * @param image: array where image data is stored
 * @tparam HEIGHT: height of image
 * @tparam WIDTH: width of image
 * @tparam T: data type (only an 8 bit data type allowed)
*/
template <unsigned int HEIGHT, unsigned int WIDTH, typename T>
int write_image_bin(const char *fileName, const T image[HEIGHT][WIDTH]){
    ofstream outfile(fileName, ios::out | ios::binary);
    if (!outfile.is_open()){
        cout << "ERROR: File " << fileName << " could not be opened!\n";
        return 1;
    }

    uint8_t buf;
    for (unsigned int i = 0; i < HEIGHT; ++i){
        for (unsigned int j = 0; j < WIDTH; ++j){
        	//Write data binary
        	buf = image[i][j];
            outfile << buf;
        }
    }
    outfile.close();
    return 0;
}

#endif /* TB_HELPERS_H_ */
