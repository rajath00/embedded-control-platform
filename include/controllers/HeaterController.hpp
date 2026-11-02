# pragma once

#include <algorithm>
#include <cassert>
#include "controllers/proportional_controller.hpp"
#include "sensors/sensor_sample.hpp"

class HeaterController {
    private:
        ProportionalController proportionalController_;
        float maximumPressure_;

    public:
        HeaterController(float setpoint, float gain, float maximumPressure)
            : proportionalController_(setpoint,gain),
            maximumPressure_(maximumPressure){}

        float calculateHeaterPower(const SensorSample& temperature, const SensorSample& pressure) const {

            if (temperature.status != SensorStatus::Connected || pressure.status != SensorStatus::Connected)
            {
                return 0.0f;
            }
            if (pressure.value>=maximumPressure_)
            {
                return 0.0f;
            }

            return proportionalController_.calculate(temperature.value);
        }
};