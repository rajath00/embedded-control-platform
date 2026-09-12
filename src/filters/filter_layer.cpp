#include "filters/filter_layer.hpp"
 #include <numeric>

 #include<iostream>

FilterLayer::FilterLayer(std::size_t windowSize)
    : windowSize_(windowSize)
{
}

std::vector<SensorSample> FilterLayer::process(const std::vector<SensorSample>& samples)
{   
    auto filteredSamples = samples;

    for (auto &sample : filteredSamples)
    {
        if(sample.status != SensorStatus::Connected)
        {
            sample.value = 0.0f;
            reset(sample.id);
        }
        else
        {
            auto &state = states_[sample.id];
            state.sum +=sample.value;
            if (state.history.size()==windowSize_)
            {
                state.sum -=state.history.front();
                state.history.pop_front();
            }  
            state.history.push_back(sample.value);

            sample.value = state.sum/static_cast<float>(state.history.size());
        }

    }
    return filteredSamples;

}



void FilterLayer::reset(const std::string& sensorId)
{
    const auto it = states_.find(sensorId);

    if (it != states_.end()) {
        it->second.history.clear();
        it->second.sum = 0.0f;
        it->second.wasConnected = false;
    }
}

void FilterLayer::resetAll()
{
    for(auto &state:states_)
    {
        reset(state.first);
    }
}

