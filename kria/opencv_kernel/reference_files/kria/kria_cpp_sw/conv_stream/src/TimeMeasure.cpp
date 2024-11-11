#include "TimeMeasure.h"

TimeMeasure::TimeMeasure(/* args */)
{
    frameCount = 0;
    startTime = std::chrono::high_resolution_clock::now();
    stopTime = std::chrono::high_resolution_clock::now();
}

TimeMeasure::~TimeMeasure()
{
}

/**
 * @brief Increment the frame counter
 * @param none
 * @return void
 */
void TimeMeasure::incFrameCount(){
    frameCount ++;
}

/**
 * @brief Get the frame rate
 * @param none
 * @return double framerate
 */
double TimeMeasure::getFrameRate(){
    // Get the time difference to the start time
    std::chrono::duration<double> elapsed = stopTime - startTime;
    // Return frame rate
    return frameCount / elapsed.count();
}

/**
 * @brief Get the number of frames
 * @param none
 * @return int number of frames
 */
int TimeMeasure::getNumberOfFrames(){
    return frameCount;
}

/**
 * @brief Get the elapsed time in seconds between start and stop time
 * @param none
 * @return double elapsed time in seconds
 */
double TimeMeasure::getTimeSecs(){
    std::chrono::duration<double> elapsed = stopTime - startTime;
    return elapsed.count();
}

/**
 * @brief Get the elapsed time in milliseconds between start and stop time
 * @param none
 * @return double elapsed time in millisecnods
 */
double TimeMeasure::getTimeMilliSecs(){
    std::chrono::duration<double> elapsed = stopTime - startTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
}

/**
 * @brief Get the elapsed time in microseconds between start and stop time
 * @param none
 * @return double elapsed time in microsecnods
 */
double TimeMeasure::getTimeMicroSecs(){
    std::chrono::duration<double> elapsed = stopTime - startTime;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

/**
 * @brief Set the start time
 * @param none
 * @return void
 */
void TimeMeasure::setStartTime(){
    startTime = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Set the stop time
 * @param none
 * @return void
 */
void TimeMeasure::setStopTime(){
    stopTime = std::chrono::high_resolution_clock::now();
}
