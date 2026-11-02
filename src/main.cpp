#include "filters/filter_layer.hpp"
#include "platform/control_platform.hpp"
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_pressure_sensor.hpp"
#include "sensors/simulated_temperature_sensor.hpp"
#include <iostream>
#include <vector>
#include "controllers/proportional_controller.hpp"
#include "controllers/HeaterController.hpp"
#include "platform/thermal_plant.hpp"

//float getplantTemperature() { return 10.0f; }

float getplantPressure() { return 400.0f; }

int main() {

    // ControlPlatform platform;

    SensorLayer sensorLayer;
    FilterLayer filerlayer(FilterType::LowPass, 5, 0.5);
    HeaterController controller(50.0f, 50.0f, 450.0f);
    ThermalPlant plant(
    10.0f, // initial temperature
    5.0f,  // ambient temperature
    5.0f,  // heater gain
    0.1f,  // cooling gain
    300.0f, // pressure at ambient, mBar
    1.0f   // mBar per degree C
    );

    sensorLayer.addSensor(std::make_unique<SimulatedTemperatureSensor>("TC0", [&plant] { return plant.temperature(); }));
    sensorLayer.addSensor(std::make_unique<SimulatedPressureSensor>("TL3",[&plant] { return plant.pressure(); }));

    auto rawSamples = sensorLayer.readAll();
    auto filteredSamples = filerlayer.process(rawSamples);



    for (size_t i = 0; i < 100; i++) {
        const SensorSample* temperature = nullptr;
        const SensorSample* pressure = nullptr;
        for (const auto &sample : filteredSamples) {
            const auto timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                         sample.timestamp.time_since_epoch())
                                         .count();
            //std::cout << timestampMs << "ms\n";
            

            if (sample.type == SensorType::Temperature) {
                temperature = &sample;
                std::cout << sample.id << ": " << sample.value << ' ' << sample.unit << '\n';
            } else if (sample.type == SensorType::Pressure) {
                pressure = &sample;
                std::cout << sample.id << ": " << sample.value << ' ' << sample.unit << '\n';
            }
        }
        float heaterPower = 0.0f;

        if (temperature != nullptr && pressure != nullptr) {
            heaterPower = controller.calculateHeaterPower(*temperature, *pressure);
        }
        std::cout<< "HeaterPower: "<< heaterPower << '\n';

        plant.advance(heaterPower,1.0f);

        rawSamples = sensorLayer.readAll();
        filteredSamples = filerlayer.process(rawSamples);

        // for (const auto &sample : rawSamples) {
        //     std::cout << sample.id << ": " << sample.value << ' ' << sample.unit << '\n';
        // }


         

        // if (filteredSamples[0].status == SensorStatus::Connected)
        // {
        //     const float heaterPower = controller.calculate(filteredSamples[0].value);
        //     std::cout<< "HeaterPower: "<< heaterPower << '\n';
        // }
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
