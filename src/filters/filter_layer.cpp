#include "filters/filter_layer.hpp"

#include <cassert>
#include <iostream>

FilterLayer::FilterLayer(FilterType type,std::size_t windowSize,float alpha)
    : type_(type), windowSize_(windowSize), alpha_{alpha}
{
    assert(windowSize > 0);
    assert(alpha >= 0.0f && alpha <= 1.0f);

}

std::vector<SensorSample> FilterLayer::process(const std::vector<SensorSample>& samples)
{   
    auto filteredSamples = samples;

    for (auto &sample : filteredSamples)
    {
        if(sample.status != SensorStatus::Connected)
        {
            reset(sample.id);
            continue;
        }

        if (type_ == FilterType::MovingAverage)
        {
            auto &state = movingAverageStates_[sample.id];
            state.sum +=sample.value;
            if (state.history.size()>=windowSize_)
            {
                state.sum -=state.history.front();
                state.history.pop_front();
            }  
            state.history.push_back(sample.value);

            sample.value = state.sum/static_cast<float>(state.history.size());
        }
        else if(type_==FilterType::LowPass)
        {
            auto &state = lowPassStates_[sample.id];

            if(!state.initialized)
            {
                state.previousValue = sample.value;
                state.initialized = true;
            }
            else
            {
                state.previousValue = alpha_*sample.value + (1.0f-alpha_) * state.previousValue;
            }

            sample.value = state.previousValue;
        }

    }
    return filteredSamples;

}


void FilterLayer::reset(const std::string& sensorId)
{
    if(type_ == FilterType::MovingAverage)
    {
        const auto it = movingAverageStates_.find(sensorId);

        if (it != movingAverageStates_.end()) {
            it->second.history.clear();
            it->second.sum = 0.0f;
         }
    }
    else if(type_ == FilterType::LowPass)
    {
        const auto it = lowPassStates_.find(sensorId);
        if (it != lowPassStates_.end())
        {
            it->second.initialized = false;
            it->second.previousValue = 0.0f;
        }
    }

}

void FilterLayer::resetAll()
{
    if(type_ == FilterType::MovingAverage)
    {
        for(auto &state:movingAverageStates_)
        {
            reset(state.first);
        }
    }
    else if(type_ == FilterType::LowPass)
    {
        for(auto &state:lowPassStates_)
        {
            reset(state.first);
        }
    }
}
