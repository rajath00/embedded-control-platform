#ifndef SENSOR_SAMPLE_HPP
#define SENSOR_SAMPLE_HPP

#include<chrono>
#include<string>


enum class SensorType
{
    Temperature,
    Pressure,
    Flow
};

enum class SensorStatus
{
    Connected,
    Disconnected,
    Invalid
};


//This is the data packet that moves between layers.
struct SensorSample {
    std::string id;
    SensorType type;
    float value;
    const char* unit;
    SensorStatus status;
    std::chrono::steady_clock::time_point timestamp;
};

#endif

//the common measurement format

/*

For example, one temperature reading could be:

id:        temperature-1
type:      Temperature
value:     24.6
unit:      °C
status:    Connected
timestamp: time when it was acquired

Using one common format means the sensor layer and filter layer can handle temperature, pressure, and flow in the same way.
SensorType identifies the kind of sensor. 
SensorStatus tells whether the sensor is connected, disconnected, or has produced an invalid reading.
*/
