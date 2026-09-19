#ifndef CONTROL_PLATFORM_HPP
#define CONTROL_PLATFORM_HPP

#include "sensors/sensor_layer.hpp"

class ControlPlatform {
  private:
    SensorLayer sensorLayer_;
    // FilterLayer filters_;
    // std::vector<std::unique_ptr<IController>> controllers_;
    // std::vector<std::unique_ptr<IService>> services_;

  public:
    void runCycle() {}
};

#endif