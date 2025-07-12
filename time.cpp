#include "time.hpp"
#include <ctime>

Time getTime(){
    std::time_t time = std::time(nullptr);
    struct tm *timeinfo = localtime(&time);
    
    Time result;
    result.year = timeinfo->tm_year + 1900;
    result.month = timeinfo->tm_mon + 1;
    result.day = timeinfo->tm_mday;
    result.hour = timeinfo->tm_hour;
    result.minute = timeinfo->tm_min;
    result.second = timeinfo->tm_sec;
    
    return result;
}