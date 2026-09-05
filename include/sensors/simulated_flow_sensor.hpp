#ifndef SIMULATED_FLOW_SENSOR_HPP
#define SIMULATED_FLOW_SENSOR_HPP

#include "sensor_source.hpp"
#include <functional>

//— simulated hardware
// This is one concrete sensor implementation. It imitates a real flow sensor.

class SimulatedFlowSensor : public ISensorSource{
    private:
        std::string id_;
        std::function<float()> trueFlow_; //This stores a function that gets the actual flow from the distributed thermal-system simulation.
        float offset_{0.0f}; //This simulates calibration error. If the true flow is 25.0 °C and offsetC_ is 0.4 °C, the sensor reports 25.4 °C.
        bool connected_{true}; //This lets you simulate a disconnected sensor.

    public:
        SimulatedFlowSensor(
        std::string id,
        std::function<float()> trueFlow)
        : id_(std::move(id)),
          trueFlow_(std::move(trueFlow)) {}


        void setOffset(float offsetC) { 
            offset_ = offsetC; 
        }
        void setConnected(bool connected) { 
            connected_ = connected; 
        }

        SensorSample acquire() override //This produces one raw sensor measurement.
        {
            return {
                id_,
                SensorType::Flow,
                connected_ ? trueFlow_() + offset_ : 0.0f, //If connected, it reports the plant flow plus the simulated error. If disconnected, it returns a placeholder value and marks the sample as disconnected.
                "m3",
                connected_ ? SensorStatus::Connected
                        : SensorStatus::Disconnected,
                {} // Sensor layer owns the timestamp
            }; //At this stage, the sensor does not filter or validate the reading. It only simulates what the hardware would return.
        }
};

#endif