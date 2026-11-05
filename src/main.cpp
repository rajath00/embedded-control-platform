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

int main() {

    ControlPlatform platform;

    for(int cycle = 0; cycle <100; ++cycle)
    {
        const auto result = platform.runCycle();
        assert(result.heaterPower >=0.0f);
        assert(result.heaterPower<=100.0f);
        std::cout << "Temp: " << result.measuredTemperature << ' ' << "Pressure: " << result.measuredPressure << '\n';
    }

    return 0;
}

/*

This keeps responsibilities clean:
- SimulatedTemperatureSensor: simulates hardware measurement.
- SensorLayer: reads sensors, applies validation, adds timestamps.
- FilterLayer: smooths or estimates from received measurements.
- DistributedThermalSystem: owns the actual simulated plant temperature, pressure, and flow.


*/
