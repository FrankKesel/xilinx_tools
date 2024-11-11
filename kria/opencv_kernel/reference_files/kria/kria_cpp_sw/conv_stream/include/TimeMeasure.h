/**
* @file TimeMeasure.h
* @author Frank Kesel
* @date 25 Jun 2024
* @version 1.0
* @brief A class for time and frame rate measurement
* @details 
*/

#include <iostream>
#include <chrono>


#ifndef TimeMeasure_h
#define TimeMeasure_h

class TimeMeasure
{
private:
    int frameCount;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
public:
    TimeMeasure(/* args */);
    ~TimeMeasure();
    void incFrameCount();
    double getFrameRate();
    int getNumberOfFrames();
    double getTimeSecs();
    double getTimeMilliSecs();
    double getTimeMicroSecs();
    void setStartTime();
    void setStopTime();
};

#endif