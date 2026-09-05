#include<iostream>
#include<vector>
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_temperature_sensor.hpp"
#include "sensors/simulated_pressure_sensor.hpp"
#include "control_platform.hpp"
#include "filter_layer.hpp"

float getplantTemperature()
{
    return 31.2f;
}

float getplantPressure()
{
    return 400.0f;
}


int main()
{
    
    // ControlPlatform platform;

    SensorLayer sensorLayer;
    FilterLayer filerlayer;

    sensorLayer.addSensor(std::make_unique<SimulatedTemperatureSensor>("TC0",getplantTemperature));
    // sensorLayer.addSensor(std::make_unique<SimulatedPressureSensor>("TL3",getplantPressure));

    auto rawSamples = sensorLayer.readAll();
    auto filteredSamples = filerlayer.process(rawSamples);

    // for(size_t i = 0; i<10;i++)
    // {   
    //     for(const auto&sample: samples)
    //     {   
    //         const auto timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
    //         sample.timestamp.time_since_epoch()).count();
    //         std::cout << timestampMs<<"ms\n";
    //         std::cout << sample.id << ": "
    //                 << sample.value << ' '
    //                 << sample.unit << '\n';
    //     }
    //     samples = sensorLayer.readAll();
    // }

    return 0;

}



/*

This keeps responsibilities clean:
- SimulatedTemperatureSensor: simulates hardware measurement.
- SensorLayer: reads sensors, applies validation, adds timestamps.
- FilterLayer: smooths or estimates from received measurements.
- DistributedThermalSystem: owns the actual simulated plant temperature, pressure, and flow.


*/
