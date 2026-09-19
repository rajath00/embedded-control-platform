# Embedded Control Platform

A C++17 learning project for building a layered thermal-control platform.
The current milestone provides validated sensor data and configurable signal
filtering, using simulated sensors.

## Current architecture

```text
Simulated sensors
      ↓
SensorLayer
      ↓
FilterLayer
      ↓
Application (main.cpp)
```

## Components

### Simulated sensors

The project provides simulated temperature, pressure, and flow sensors that
implement a shared `ISensorSource` interface. They make it possible to develop
and test the platform without physical hardware.

### SensorLayer

`SensorLayer` owns registered sensor sources and reads them as `SensorSample`
objects.

It:

- collects measurements from all registered sensors;
- adds an acquisition timestamp;
- marks disconnected, NaN, and infinite readings as `Invalid`.

### FilterLayer

`FilterLayer` processes samples produced by `SensorLayer`. It maintains
independent state for each sensor ID and supports:

- moving-average filtering;
- low-pass filtering;
- state reset when a sample is invalid or a sensor disconnects.

The filter layer does not communicate with sensors directly.

### Tests

Unit tests cover sensor validation and filter behavior, including independent
state for multiple sensors and reset behavior after invalid input.

## Build and test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Next milestone

Split the build into sensor and filter libraries, then add the control layer:

```text
sensor → validate → filter → control command → actuator simulation
```

The first controller will use filtered temperature to calculate a bounded
heater-power command. PID control, fault states, and a simulated thermal plant
will follow.
