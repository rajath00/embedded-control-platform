#pragma once

#include<vector>
#include "sensors/sensor_layer.hpp"
#include<deque>
#include<unordered_map>

//process already-valid measurements

struct MovingAverageState {
    std::deque<float> history;
    float sum{0.0f};
    bool wasConnected{false};
};

class FilterLayer {

    private:

        std::size_t windowSize_;
        std::unordered_map<std::string, MovingAverageState> states_;

    public:

        explicit FilterLayer(std::size_t windowSize = 5);

        std::vector<SensorSample> process(const std::vector<SensorSample>& samples);       
        //The filter receives sensor-layer output. It does not access hardware or simulated sensors directly.
        
        void reset(const std::string& sensorId);
        void resetAll();
};