#include "filters/filter_layer.hpp"
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_temperature_sensor.hpp"

#include "test_helpers.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

SensorSample sample(const std::string &id, float value,
                    SensorStatus status = SensorStatus::Connected) {
    return SensorSample{id, SensorType::Temperature, value, "C", status, {}};
}

int main() {
    // 1. Single sensor: average grows as readings arrive.
    {
        FilterLayer filter(FilterType::MovingAverage, 3);
        auto result = filter.process({sample("temp-1", 10.0f)});

        assert(result.size() == 1);
        nearlyEqual(result[0].value, 10.0f, "Filtered value should be 10");

        result = filter.process({sample("temp-1", 25.0f)});
        nearlyEqual(result[0].value, (10 + 25) / 2.0f, "Window should average 10 and 25");
    }

    // 2. Multiple sensors: each sensor has its own filter history

    {
        FilterLayer filter(FilterType::MovingAverage, 3);

        auto result = filter.process({sample("temp-1", 10.0f), sample("pressure-1", 100.0f)});

        nearlyEqual(result[0].value, 10.0f, "The expected value should be 10");
        nearlyEqual(result[1].value, 100.0f, "The expected value should be 100");

        result = filter.process({sample("temp-1", 20.0f), sample("pressure-1", 90.0f)});

        nearlyEqual(result[0].value, 15.0f, "The window should average 10 and 20");
        nearlyEqual(result[1].value, 95.0f, "The window should average 100 and 90");
    }

    // 3. Invalid data: It is not filtered and must not enter history
    {
        FilterLayer filter(FilterType::MovingAverage, 3);
        auto result = filter.process({sample("temp-1", 10.0f, SensorStatus::Invalid)});

        nearlyEqual(result[0].value, 10.0f, "The value must be reset to 10");
        assert(result[0].status == SensorStatus::Invalid);
    }

    // 4. Disconnection then reconnection: start a fresh window

    {
        FilterLayer filter(FilterType::MovingAverage, 3);
        filter.process({sample("temp-1", 10.0f)});
        filter.process({sample("temp-1", 20.0f)});

        auto disconnected = filter.process({sample("temp-1", 0.0f, SensorStatus::Disconnected)});

        assert(disconnected[0].status == SensorStatus::Disconnected);

        auto connected = filter.process({sample("temp-1", 50.0f)});

        nearlyEqual(connected[0].value, 50.0f, "The expected value should be 50");
    }

    // 5. full window with a size 3. oldest value is removed.
    {
        FilterLayer filter(FilterType::MovingAverage, 3);
        nearlyEqual(filter.process({sample("temp-1", 10.0f)})[0].value, 10.0f,
                    "The initial value must be 10");
        nearlyEqual(filter.process({sample("temp-1", 20.0f)})[0].value, 15.0f,
                    "The window should average 10 and 20");
        nearlyEqual(filter.process({sample("temp-1", 30.0f)})[0].value, 20.0f,
                    "The window should average 10, 20 and 30");

        // Window changes from [10, 20, 30] to [20, 30, 40].
        nearlyEqual(filter.process({sample("temp-1", 40.0f)})[0].value, 30.0f,
                    "The window should average 20, 30 and 40");
    }

    // first value initializes output directly;
    // known alpha calculation;
    // separate state for multiple sensors;
    // invalid/disconnected input resets state;
    // reconnection starts with the new value.

    // 6. Low pass filter: first value initializes output directly;
    {
        FilterLayer filter(FilterType::LowPass, 5, 0.5);
        auto init = filter.process({sample("temp-1", 10.0f)});
        nearlyEqual(init[0].value, 10.0f, "Filtered value must pass unchanged");
        assert(init[0].status == SensorStatus::Connected);
    }

    // 7. verify a known alpha calculation
    {
        FilterLayer filter(FilterType::LowPass, 5, 0.25f);

        filter.process({sample("temp-1", 20.0f)});
        auto result = filter.process({sample("temp-1", 10.0f)});

        // 0.25*10 + (1-0.25)*20 = 17.5
        nearlyEqual(result[0].value, 17.5f, "Low pass output should use alpha = 0.25");
    }

    // Low-pass 3: every sensor has independent filter state.
    {
        FilterLayer filter(FilterType::LowPass, 5, 0.5f);

        filter.process({sample("temp-1", 20.0f), sample("pressure-1", 100.0f)});

        auto result = filter.process({sample("temp-1", 30.0f), sample("pressure-1", 200.0f)});

        nearlyEqual(result[0].value, (0.5 * 20 + 0.5 * 30), "Temp must use its own low-pass state");
        nearlyEqual(result[1].value, (0.5 * 100 + 0.5 * 200),
                    "Pressure must use its own low-pass state");
    }

    // Low-pass 4: invalid input resets the sensor state.

    {
        FilterLayer filter(FilterType::LowPass, 5, 0.25f);
        filter.process({sample("temp-1", 20.0f)});
        auto valid = filter.process({sample("temp-1", 10.0f)});

        nearlyEqual(valid[0].value, (0.25 * 10 + 0.75 * 20), "The sensor values must be filtered");

        auto invalid = filter.process({sample("temp-1", 999.0f, SensorStatus::Invalid)});

        assert(invalid[0].status == SensorStatus::Invalid);
        nearlyEqual(invalid[0].value, 999.0f, "Invalid sample value must be preserved");

        auto recovered = filter.process({sample("temp-1", 40.0f)});

        // State was reset. Do not mix 40 with the old output 22.
        nearlyEqual(recovered[0].value, 40.0f,
                    "First valid sample after invalid data must start fresh");
    }

    // Low-pass 5: disconnect also resets state, and reconnection starts fresh
    {
        FilterLayer filter(FilterType::LowPass, 5, 0.25f);
        filter.process({sample("temp-1", 20.0f)});
        auto valid = filter.process({sample("temp-1", 10.0f)});

        nearlyEqual(valid[0].value, (0.25 * 10 + 0.75 * 20), "The sensor values must be filtered");

        auto disconnected = filter.process({sample("temp-1", 0.0f, SensorStatus::Disconnected)});

        assert(disconnected[0].status == SensorStatus::Disconnected);

        auto reconnected = filter.process({sample("temp-1", 50.0f)});

        nearlyEqual(reconnected[0].value, 50.0f,
                    "First sample after reconnection must start fresh");
    }
}