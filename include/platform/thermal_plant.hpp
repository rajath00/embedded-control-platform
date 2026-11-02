#pragma once

#include <algorithm>

class ThermalPlant{
    private:
    float temperature_;
    float ambientTemperature_;
    float heaterGain_;
    float coolingGain_;
    float pressureAtAmbient_;
    float pressurePerDegree_;

    public:
     ThermalPlant(float initialTemperature, float ambientTemperature, 
                    float heaterGain, float coolingGain, float pressureAtAmbient, float pressurePerDegree)
        : temperature_(initialTemperature), ambientTemperature_(ambientTemperature),
        heaterGain_(heaterGain), coolingGain_(coolingGain), pressureAtAmbient_(pressureAtAmbient),pressurePerDegree_(pressurePerDegree) {}

    
    float temperature() const {
            return temperature_;
    }

    float pressure() const {
        return pressureAtAmbient_ + pressurePerDegree_ * (temperature_ - ambientTemperature_);
    }

    void advance (float heaterPowerPercent, float dt) {
        const float heaterPower = std::clamp(heaterPowerPercent, 0.0f, 100.0f);
        const float heating = heaterGain_ * (heaterPower / 100.0f);
        const float cooling = coolingGain_ * (temperature_ - ambientTemperature_);

        temperature_ += (heating - cooling) * dt;
    }
};