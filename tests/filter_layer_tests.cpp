#include "filters/filter_layer.hpp"
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_temperature_sensor.hpp"

#include <cassert>
#include <cmath>
#include <memory>
#include <iostream>


constexpr float EPSILON = 0.001f;

void nearlyEqual(float actual, float expected,const char* message)
{   
    if (std::fabs(actual - expected) >= EPSILON)
    {
        std::cerr << "Test failed: " << message << '\n'
            << "  Expected: " << expected << '\n'
            << "  Actual:   " << actual << '\n';
    }
    assert(std::fabs(actual - expected) < EPSILON);
}

SensorSample sample(const std::string& id, float value,
                    SensorStatus status = SensorStatus::Connected)
{
    return SensorSample{
        id,
        SensorType::Temperature,
        value,
        "C",
        status,
        {}
    };
}

int main()
{
    // 1. Single sensor: average grows as readings arrive.
    {
        FilterLayer filter(3);
        auto result = filter.process({sample("temp-1",10.0f)});

        assert(result.size() == 1);
        nearlyEqual(result[0].value,10.0f,"Filtered value should be 10");

        result = filter.process({sample("temp-1",25.0f)});
        nearlyEqual(result[0].value,(10+25)/2.0f,"Window should average 10 and 25");
    }

// 2. Multiple sensors: each sensor has its own filter history

    {
        FilterLayer filter(3);

        auto result = filter.process({sample("temp-1",10.0f),sample("pressure-1",100.0f)});

        nearlyEqual(result[0].value,10.0f,"The expected value should be 10");
        nearlyEqual(result[1].value,100.0f,"The expected value should be 100");

        result = filter.process({sample("temp-1",20.0f),sample("pressure-1",90.0f)});

        nearlyEqual(result[0].value,15.0f,"The window should average 10 and 20");
        nearlyEqual(result[1].value,95.0f,"The window should average 100 and 90");

    }

// 3. Invalid data: It is not filtered and must not enter history
    {
        FilterLayer filter(3);
        auto result = filter.process({sample("temp-1",10.0f,SensorStatus::Invalid)});

        nearlyEqual(result[0].value,0,"The value must be reset to 0");
        assert(result[0].status == SensorStatus::Invalid);

    }

// 4. Disconnection then reconnection: start a fresh window

    {
        FilterLayer filter(3);
        filter.process({sample("temp-1",10.0f)});
        filter.process({sample("temp-1",20.0f)});

        auto disconnected = filter.process({sample("temp-1",0.0f,SensorStatus::Disconnected)});

        assert(disconnected[0].status == SensorStatus::Disconnected);

        auto connected = filter.process({sample("temp-1",50.0f)});

        nearlyEqual(connected[0].value,50.0f,"The expected value should be 50");
    }

// 5. full window with a size 3. oldest value is removed.
    {
        FilterLayer filter(3);
        nearlyEqual(filter.process({sample("temp-1", 10.0f)})[0].value, 10.0f,"The initial value must be 10");
        nearlyEqual(filter.process({sample("temp-1", 20.0f)})[0].value, 15.0f,"The window should average 10 and 20");
        nearlyEqual(filter.process({sample("temp-1", 30.0f)})[0].value, 20.0f,"The window should average 10, 20 and 30");

        // Window changes from [10, 20, 30] to [20, 30, 40].
        nearlyEqual(filter.process({sample("temp-1", 40.0f)})[0].value, 30.0f,"The window should average 20, 30 and 40");

    }
}