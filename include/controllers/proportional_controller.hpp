#pragma once

#include <algorithm>
#include <cassert>

class ProportionalController
{
    private:
        float setpoint_;
        float gain_;
    public:
        ProportionalController(float setpoint, float gain):
            setpoint_(setpoint), gain_(gain) {
                assert(gain>=0.0f);
            }

        
        float calculate(float measurement) const {
                const float error = setpoint_ - measurement;
                const float power = gain_ * error;

                return std::clamp(power,0.0f,100.0f);
            }

        void setSetpoint(float setpoint) {
            setpoint_ = setpoint;
        }
};
