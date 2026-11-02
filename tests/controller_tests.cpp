#include "controllers/proportional_controller.hpp"
#include <cassert>

int main()
{

    ProportionalController controller(25.0f, 10.0f);

    assert(controller.calculate(25.0f) == 0.0f);  // target reached
    assert(controller.calculate(20.0f) == 50.0f); // 5 C below target
    assert(controller.calculate(10.0f) == 100.0f); // capped at 100%
    assert(controller.calculate(30.0f) == 0.0f);  // heater cannot cool

    return 0;
}