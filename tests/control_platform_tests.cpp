#include "platform/control_platform.hpp"

#include <cassert>


int main()
{
    {
    ControlPlatform platform;
    float firstTemperature = 0.0f;
    float lastTemperature = 0.0f;

    for (int cycle = 0; cycle < 20; ++cycle) {
        const auto result = platform.runCycle();

        assert(result.heaterPower >= 0.0f);
        assert(result.heaterPower <= 100.0f);

        if (cycle == 0) {
            firstTemperature = result.measuredTemperature;
        }

        lastTemperature = result.measuredTemperature;
    }

    assert(lastTemperature > firstTemperature);

    }

    {

    ControlPlatformConfig config;
    config.maximumPressure = 350.0f;
    config.pressureAtAmbient = 400.0f;

    ControlPlatform platform(config);

    const auto result = platform.runCycle();

    assert(result.measuredPressure >= 350.0f);
    assert(result.heaterPower == 0.0f);


    }

    // Temperature sensor fault: heater must be off.

    {
    ControlPlatformConfig config;
    config.temperatureSensorConnected = false;

    ControlPlatform platform(config);
    const auto result = platform.runCycle();

    assert(result.heaterPower == 0.0f);
    }
    
    // Pressure sensor fault: heater must be off.
    {
        ControlPlatformConfig config;
        config.pressureSensorConnected = false;

        ControlPlatform platform(config);
        const auto result = platform.runCycle();

        assert(result.heaterPower == 0.0f);
    }

    return 0;


}