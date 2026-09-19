#include "sensors/fake_sensor.hpp"
#include "sensors/sensor_layer.hpp"
#include "sensors/simulated_temperature_sensor.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

#include "test_helpers.hpp"

SensorSample makeSample(const std::string &id, SensorType type, float value, const char *unit,
                        SensorStatus status = SensorStatus::Connected) {
    return SensorSample{id, type, value, unit, status, {}};
}

int main() {
    // 1. No sensors returns no samples

    {
        SensorLayer layer;

        auto samples = layer.readAll();

        assert(samples.empty() && "Empty layer must not return any samples");
    }

    // 2. A valid sensor retains its data and recieves a timestamp
    {
        SensorLayer layer;
        layer.addSensor(std::make_unique<FakeSensor>(
            makeSample("temp-1", SensorType::Temperature, 35.0f, "C")));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);

        const auto &sample = samples[0];

        assert(sample.id == "temp-1");
        assert(sample.type == SensorType::Temperature);
        nearlyEqual(sample.value, 35.0f, "Valid sensor value must be preserved");
        assert(sample.status == SensorStatus::Connected);
        assert(sample.timestamp != std::chrono::steady_clock::time_point{});
    }
    // 3. Multiple sensors retain their own data and order
    {

        SensorLayer layer;

        layer.addSensor(std::make_unique<FakeSensor>(
            makeSample("temp-1", SensorType::Temperature, 25.0f, "C")));

        layer.addSensor(std::make_unique<FakeSensor>(
            makeSample("pressure-1", SensorType::Pressure, 101.3f, "kPa")));

        layer.addSensor(
            std::make_unique<FakeSensor>(makeSample("flow-1", SensorType::Flow, 2.5f, "L/min")));

        const auto samples = layer.readAll();

        assert(samples.size() == 3);

        assert(samples[0].id == "temp-1");
        assert(samples[0].type == SensorType::Temperature);
        nearlyEqual(samples[0].value, 25.0f, "Temperature value must be preserved");

        assert(samples[1].id == "pressure-1");
        assert(samples[1].type == SensorType::Pressure);
        nearlyEqual(samples[1].value, 101.3f, "Pressure value must be preserved");

        assert(samples[2].id == "flow-1");
        assert(samples[2].type == SensorType::Flow);
        nearlyEqual(samples[2].value, 2.5f, "Flow value must be preserved");
    }

    // 4. A disconnected sensor becomes invalid
    {

        SensorLayer layer;

        layer.addSensor(std::make_unique<FakeSensor>(makeSample(
            "temp-1", SensorType::Temperature, 123.0f, "C", SensorStatus::Disconnected)));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);
        assert(samples[0].status == SensorStatus::Invalid &&
               "Disconnected sensor must be unusable");

        // SensorLayer changes status, but does not overwrite the raw value.
        nearlyEqual(samples[0].value, 123.0f, "SensorLayer must preserve raw disconnected value");
    }

    // 5.NaN becomes invalid
    {
        SensorLayer layer;

        layer.addSensor(std::make_unique<FakeSensor>(makeSample(
            "temp-1", SensorType::Temperature, std::numeric_limits<float>::quiet_NaN(), "C")));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);
        assert(samples[0].status == SensorStatus::Invalid && "NaN measurement must be invalid");
    }

    // 6. Infinity becomes invalid

    {
        SensorLayer layer;

        layer.addSensor(std::make_unique<FakeSensor>(makeSample(
            "pressure-1", SensorType::Pressure, std::numeric_limits<float>::infinity(), "kPa")));

        const auto samples = layer.readAll();

        assert(samples.size() == 1);
        assert(samples[0].status == SensorStatus::Invalid &&
               "Infinity measurement must be invalid");
    }
}
