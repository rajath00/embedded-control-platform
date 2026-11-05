#ifndef CONTROL_PLATFORM_HPP
#define CONTROL_PLATFORM_HPP

#include "sensors/sensor_layer.hpp"
#include "filters/filter_layer.hpp"
#include "controllers/HeaterController.hpp"
#include "platform/thermal_plant.hpp"
#include "sensors/simulated_pressure_sensor.hpp"
#include "sensors/simulated_temperature_sensor.hpp"

struct ControlCycleResult {
    float heaterPower;
    float measuredTemperature;
    float measuredPressure;
};

struct ControlPlatformConfig {
    float setpoint{50.0f};
    float controllerGain{50.0f};
    float maximumPressure{450.0f};

    FilterType filterType{FilterType::LowPass};
    std::size_t filterWindowSize{5};
    float filterAlpha{0.5f};

    float initialTemperature{10.0f};
    float ambientTemperature{5.0f};
    float heaterGain{5.0f};
    float coolingGain{0.1f};
    bool temperatureSensorConnected{true};

    float pressureAtAmbient{300.0f};
    float pressurePerDegree{1.0f};
    bool pressureSensorConnected{true};
};

class ControlPlatform {
  private:
    SensorLayer sensorLayer_;
    FilterLayer filterlayer_;
    HeaterController heaterController_;
    ThermalPlant plant_;

  public:

ControlPlatform(const ControlPlatformConfig& config = {})
    : filterlayer_(
          config.filterType,
          config.filterWindowSize,
          config.filterAlpha),
      heaterController_(
          config.setpoint,
          config.controllerGain,
          config.maximumPressure),
      plant_(
          config.initialTemperature,
          config.ambientTemperature,
          config.heaterGain,
          config.coolingGain,
          config.pressureAtAmbient,
          config.pressurePerDegree) {

        auto temperatureSensor = std::make_unique<SimulatedTemperatureSensor>(
        "TC0",
        [this] { return plant_.temperature();});


        temperatureSensor->setConnected(config.temperatureSensorConnected);
        sensorLayer_.addSensor(std::move(temperatureSensor));

        auto pressureSensor = std::make_unique<SimulatedPressureSensor>(
            "TL3",
            [this] { return plant_.pressure(); }
        );
        pressureSensor->setConnected(config.pressureSensorConnected);
        sensorLayer_.addSensor(std::move(pressureSensor));

    }

    ControlCycleResult runCycle() {

        ControlCycleResult result{0.0f, 0.0f, 0.0f};

        auto rawSamples = sensorLayer_.readAll();
        auto filteredSamples = filterlayer_.process(rawSamples);

        const SensorSample* temperature = nullptr;
        const SensorSample* pressure = nullptr;
        for (const auto &sample : filteredSamples) {
            if (sample.type == SensorType::Temperature) {
                temperature = &sample;
            } else if (sample.type == SensorType::Pressure) {
                pressure = &sample;
            }
        }

        float heaterPower = 0.0f;

        if (temperature != nullptr && pressure != nullptr) {
            heaterPower = heaterController_.calculateHeaterPower(*temperature, *pressure);
            result.measuredPressure = pressure->value;
            result.measuredTemperature = temperature->value;
        }
        result.heaterPower = heaterPower;
        plant_.advance(heaterPower,1.0f);
        return result;


    }
};

#endif