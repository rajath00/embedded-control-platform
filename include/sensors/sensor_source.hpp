#ifndef SENSOR_SOURCE_HPP
#define SENSOR_SOURCE_HPP



#include "sensor_sample.hpp"



class ISensorSource {
public:
    virtual ~ISensorSource() = default;

    virtual SensorSample acquire() = 0; 
    //acquire function. It has a return type of Sensor Sample. Any Sensor that is derived from this class must have an acquire function.
    // Withought this function it cannot be created.

};

#endif



/*

This says: every sensor source must provide an acquire() function.
The sensor layer does not need to know whether it is reading a:
- SimulatedTemperatureSensor
- SimulatedPressureSensor
- real I2C temperature sensor
- real ADC flow sensor
It only knows that it can call:

sensor->acquire();

and receive a SensorSample.

This is the abstraction boundary between the sensor layer and the individual sensor implementations.

*/