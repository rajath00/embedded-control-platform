#pragma once

#include "sensors/sensor_layer.hpp"
#include <deque>
#include <unordered_map>
#include <vector>

// process already-valid measurements

struct MovingAverageState {
    std::deque<float> history;
    float sum{0.0f};
};

enum class FilterType { MovingAverage, LowPass };

struct LowPassState {
    float previousValue{0.0f};
    bool initialized{false};
};

class FilterLayer {

  private:
    FilterType type_;
    std::size_t windowSize_;
    float alpha_{0.0f};

    std::unordered_map<std::string, LowPassState> lowPassStates_;
    std::unordered_map<std::string, MovingAverageState> movingAverageStates_;

  public:
    explicit FilterLayer(FilterType type = FilterType::MovingAverage, std::size_t windowSize = 5,
                         float alpha = 0.5f);

    std::vector<SensorSample> process(const std::vector<SensorSample> &samples);
    // The filter receives sensor-layer output. It does not access hardware or simulated sensors
    // directly.

    void reset(const std::string &sensorId);
    void resetAll();

    static FilterLayer makeMovingAverage(std::size_t windowSize = 5);

    static FilterLayer makeLowPass(float alpha = 0.5f);
};