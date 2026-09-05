#ifndef SIMULATED_TEMPERATURE_SENSOR_HPP
#define SIMULATED_TEMPERATURE_SENSOR_HPP

#include "sensor_source.hpp"
#include <functional>
#include <random>

//— simulated hardware
// This is one concrete sensor implementation. It imitates a real temperature sensor.

class SimulatedTemperatureSensor : public ISensorSource{
    private:
        std::string id_;
        std::function<float()> trueTemperature_; //This stores a function that gets the actual temperature from the distributed thermal-system simulation.
        float offsetC_{0.0f}; //This simulates calibration error. If the true temperature is 25.0 °C and offsetC_ is 0.4 °C, the sensor reports 25.4 °C.
        bool connected_{true}; //This lets you simulate a disconnected sensor.

    public:
        SimulatedTemperatureSensor(
        std::string id,
        std::function<float()> trueTemperature)
        : id_(std::move(id)),
          trueTemperature_(std::move(trueTemperature)) {}


        void setOffset(float offsetC) {  //for fixed offset calibration bias
            offsetC_ = offsetC; 
        }
        void setConnected(bool connected) { 
            connected_ = connected; 
        }

        SensorSample acquire() override //This produces one raw sensor measurement.
        {
            static std::mt19937 generator{std::random_device{}()};
            static std::normal_distribution<float> noise{0.0f, 0.2f};
            return {
                id_,
                SensorType::Temperature,
                connected_ ? trueTemperature_() + offsetC_ + noise(generator) : 0.0f, //If connected, it reports the plant temperature plus the simulated error. If disconnected, it returns a placeholder value and marks the sample as disconnected.
                "°C",
                connected_ ? SensorStatus::Connected
                        : SensorStatus::Disconnected,
                {} // Sensor layer owns the timestamp
            }; //At this stage, the sensor does not filter or validate the reading. It only simulates what the hardware would return.
        }
};

#endif