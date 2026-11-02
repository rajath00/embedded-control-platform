#ifndef FLOW_SENSOR_HPP
#define FLOW_SENSOR_HPP

#include "sensor.hpp"

class FlowSensor: public Sensor<float>
{
    public:
        FlowSensor()
            : Sensor<float>{0.0f,100.0f} {}
        
        const char* unit() const
        {
            return "L/min";
        }
};


#endif