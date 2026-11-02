#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP


#include "sensor.hpp"

class PressureSensor : public Sensor<float>
{
    public:
        PressureSensor() 
            : Sensor<float>{0.0f,10.0f}{}

        const char* unit() const
        {
            return "bar";
        }
};



#endif