#ifndef FAKE_SENSOR_HPP
#define FAKE_SENSOR_HPP

#include "sensor_source.hpp"
#include <functional>

class FakeSensor : public ISensorSource {
  public:
    explicit FakeSensor(SensorSample sample) : sample_(sample) {}

    SensorSample acquire() override { return sample_; }

  private:
    SensorSample sample_;
};

#endif