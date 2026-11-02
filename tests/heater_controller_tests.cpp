#include "controllers/HeaterController.hpp"

#include <cassert>


SensorSample makeSample(const std::string &id, SensorType type, float value, const char *unit,
                        SensorStatus status = SensorStatus::Connected) {
    return SensorSample{id, type, value, unit, status, {}};
}


int main() {
    // setpoint: 25 C, gain: 10 % per C, max safe pressure: 300
    HeaterController controller(25.0f, 10.0f, 300.0f);

    // At setpoint: no heat is required.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 25.0f, "C"), makeSample("press-1", SensorType::Pressure, 100.0f, "mbar")) == 0.0f);

    // 5 C below target: 10 * (25 - 20) = 50%.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 20.0f, "C"), makeSample("press-1", SensorType::Pressure, 100.0f, "mbar")) == 50.0f);

    // Far below target: requested 150%, clamped to 100%.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 10.0f, "C"), makeSample("press-1", SensorType::Pressure, 100.0f, "mbar")) == 100.0f);

    // Above target: heater cannot cool, so output is 0%.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 30.0f, "C"), makeSample("press-1", SensorType::Pressure, 100.0f, "mbar")) == 0.0f);

    // Pressure at the safety limit disables the heater.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 20.0f, "C"), makeSample("press-1", SensorType::Pressure, 300.0f, "mbar")) == 0.0f);

    // Pressure above the safety limit also disables the heater.
    assert(controller.calculateHeaterPower(makeSample("temp-1", SensorType::Temperature, 20.0f, "C"), makeSample("press-1", SensorType::Pressure, 350.0f, "mbar")) == 0.0f);

    return 0;
}