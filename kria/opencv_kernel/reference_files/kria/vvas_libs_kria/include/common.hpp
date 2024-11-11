/**
 * @file common.hpp
 * @author Andreas Schmid
 * @date 11 Jan 2024
 * @version 1.0
 * @brief Classes for performance measurement
 * @details
 *
 */
#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <chrono>
#include <optional>

class PerfTimer {
public:
    using time_pt_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    PerfTimer(bool start = true) :
        m_started(start)
    {
        m_start_time = std::chrono::high_resolution_clock::now();
    }

    std::optional<std::chrono::microseconds> get()
    {
        if(!m_started)
        {
            m_started = true;
            m_start_time = std::chrono::high_resolution_clock::now();
            return {};
        }
        else
        {
            return { std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_start_time) };
        }
    }

private:
    time_pt_t m_start_time;
    bool m_started;
};

template <typename T>
class RunningAverage {
public:
    RunningAverage() : m_current_avrg(0), m_count(0) {}

    T get() const
    {
        return m_current_avrg;
    }

    void update(const T& new_val)
    {
        m_count++;
        m_current_avrg = (m_current_avrg * (m_count - 1) + new_val) / m_count;
    }

private:
    T m_current_avrg;
    size_t m_count;
};

#endif // __COMMON_HPP__
