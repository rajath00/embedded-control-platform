#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include "sensor.hpp"

class TemperatureSensor: public Sensor<float>
{
    public:
        TemperatureSensor()
            : Sensor<float>{-40.0f,125.0f} {}
        
        const char* unit() const
        {
            return "°C";
        }
};


#endif