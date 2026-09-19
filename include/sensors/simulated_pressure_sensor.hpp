#ifndef SIMULATED_PRESSURE_SENSOR_HPP
#define SIMULATED_PRESSURE_SENSOR_HPP

#include "sensor_source.hpp"
#include <functional>
#include <random>

class SimulatedPressureSensor : public ISensorSource {
  private:
    std::string id_;
    std::function<float()> truePressure_;
    float offset_{0.0f};
    bool connected_{true};

  public:
    SimulatedPressureSensor(std::string id, std::function<float()> truePressure)
        : id_(std::move(id)), truePressure_(std::move(truePressure)) {}

    void setOffset(float offset) { offset_ = offset; }

    void setConnected(bool connected) { connected_ = connected; }

    SensorSample acquire() override {

        static std::mt19937 generator{std::random_device{}()};
        static std::normal_distribution<float> noise{0.0f, 1.5f};

        return {id_,
                SensorType::Pressure,
                connected_ ? truePressure_() + offset_ + noise(generator)
                           : 0.0f, // If connected, it reports the plant temperature plus the
                                   // simulated error. If disconnected, it returns a placeholder
                                   // value and marks the sample as disconnected.
                "mBar",
                connected_ ? SensorStatus::Connected : SensorStatus::Disconnected,
                {}};
    }
};

#endif