#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_temperature_sensor.hpp"
#include "sensors/fake_sensor.hpp"

#include <cassert>
#include <cmath>
#include <memory>


int main()
{
    {
        SensorLayer layer;

        layer.addSensor(std::make_unique<SimulatedTemperatureSensor>("temp-1",[]{return 25.0f;}));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);

        const auto& sample = samples[0];

        assert(sample.id == "temp-1");
        assert(sample.type == SensorType::Temperature);
        assert(sample.value == 25.0f);
        assert(sample.status == SensorStatus::Connected);
        assert(sample.timestamp != std::chrono::steady_clock::time_point{});
    }

    {

        SensorLayer layer;

        auto sensor = std::make_unique<SimulatedTemperatureSensor>("temp-1",[]{return 25.0f;});

        sensor->setConnected(false);
        layer.addSensor(std::move(sensor));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);
        assert(samples[0].status == SensorStatus::Invalid);

    }


    {

        SensorLayer layer;

        SensorSample badSample{
            "temp-1",
            SensorType::Temperature,
            std::numeric_limits<float>::quiet_NaN(),
            "°C",
            SensorStatus::Connected,
            {}
        };
        
        layer.addSensor(std::make_unique<FakeSensor>(badSample));


        const auto samples = layer.readAll();

        assert(samples.size() == 1);
        assert(samples[0].status == SensorStatus::Invalid);

    }




    
}
