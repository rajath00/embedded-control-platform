#include "filters/filter_layer.hpp"
#include "platform/control_platform.hpp"
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_pressure_sensor.hpp"
#include "sensors/simulated_temperature_sensor.hpp"
#include <iostream>
#include <vector>

float getplantTemperature() { return 10.0f; }

float getplantPressure() { return 400.0f; }

int main() {

    // ControlPlatform platform;

    SensorLayer sensorLayer;
    FilterLayer filerlayer(FilterType::LowPass, 5, 0.5);

    sensorLayer.addSensor(std::make_unique<SimulatedTemperatureSensor>("TC0", getplantTemperature));
    // sensorLayer.addSensor(std::make_unique<SimulatedPressureSensor>("TL3",getplantPressure));

    auto rawSamples = sensorLayer.readAll();
    auto filteredSamples = filerlayer.process(rawSamples);

    for (size_t i = 0; i < 3; i++) {
        for (const auto &sample : filteredSamples) {
            const auto timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                         sample.timestamp.time_since_epoch())
                                         .count();
            std::cout << timestampMs << "ms\n";
            std::cout << sample.id << ": " << sample.value << ' ' << sample.unit << '\n';
        }

        for (const auto &sample : rawSamples) {
            std::cout << sample.id << ": " << sample.value << ' ' << sample.unit << '\n';
        }
        rawSamples = sensorLayer.readAll();
        filteredSamples = filerlayer.process(rawSamples);
    }

    // FilterLayer filter(FilterType::LowPass,5,0.5);
    // auto init = filter.process(rawSamples);

    // std::cout<<init[0].value<<std::endl;

    // filter.process(rawSamples);
    // nearlyEqual(init[1].value,15.0f,"Filtered value must be 15");

    return 0;
}

/*

This keeps responsibilities clean:
- SimulatedTemperatureSensor: simulates hardware measurement.
- SensorLayer: reads sensors, applies validation, adds timestamps.
- FilterLayer: smooths or estimates from received measurements.
- DistributedThermalSystem: owns the actual simulated plant temperature, pressure, and flow.


*/
