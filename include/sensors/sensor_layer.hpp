// sensor_layer.hpp
#pragma once
#include "sensor_source.hpp"
#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

class SensorLayer {
private:
    std::vector<std::unique_ptr<ISensorSource>> sensors_; //The sensor layer owns a list of sensor objects. Because the list contains ISensorSource pointers, it can store different sensor types together

public:

    SensorLayer() = default;
    SensorLayer(const SensorLayer&) = delete;
    SensorLayer& operator=(const SensorLayer&) = delete;

    SensorLayer(SensorLayer&&) noexcept = default;
    SensorLayer& operator=(SensorLayer&&) noexcept = default;

    void addSensor(std::unique_ptr<ISensorSource> sensor) //This adds one sensor to the layer.
    {
        sensors_.push_back(std::move(sensor));
    }

    std::vector<SensorSample> readAll() //This reads every registered sensor and returns all measurements as a vector.
    {
        std::vector<SensorSample> samples;

        for (auto& sensor : sensors_) {
            SensorSample sample = sensor->acquire(); //This gets the raw measurement from one sensor.
            sample.timestamp = std::chrono::steady_clock::now(); //The sensor layer adds the time at which the measurement was acquired.

            /*
            This performs basic validation:
            - a disconnected sensor is not usable;
            - NaN and infinity are not valid measurements.
            You could later add range validation here, such as rejecting a temperature below -40 °C or above 125 °C.
            */
            if (sample.status != SensorStatus::Connected ||
                !std::isfinite(sample.value)) {
                sample.status = SensorStatus::Invalid;
            }

            samples.push_back(sample);
        }

        return samples;
    }
};
