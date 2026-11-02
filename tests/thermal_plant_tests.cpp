#include "platform/thermal_plant.hpp"

#include "test_helpers.hpp"

int main()
{   
    // 1. No heat: a plant above ambient cools.
    {
    ThermalPlant plant(20.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);

    plant.advance(0.0f,1.0f);
    // cooling = 0.1 * (20-10) = 1
    // next temp = 20-1 = 19
    nearlyEqual(plant.temperature(), 19.0f,"Plant that is above ambient must cool when heater is off");
    }

    // 2. At ambient, zero heater power leaves temperature unchanged

    {
        ThermalPlant plant(10.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);
        plant.advance(0.0f,1.0f);
        nearlyEqual(plant.temperature(), 10.0f,
                    "Plant at ambient must stay at ambient with heater off");
    }

    // 3. Full heater power raises temperature.
    {
        ThermalPlant plant(10.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);

        plant.advance(100.0f, 1.0f);

        // heating = 5 * (100 / 100) = 5
        // cooling = 0, so next temperature = 15
        nearlyEqual(plant.temperature(), 15.0f,
                    "Full heater power must raise temperature");
    }

    // 4. Heater power is clamped to 100%.
    {
        ThermalPlant plant(10.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);

        plant.advance(150.0f, 1.0f);

        nearlyEqual(plant.temperature(), 15.0f,
                    "Power above 100 percent must behave as 100 percent");
    }

    // 5. Negative heater power is clamped to 0%.
    {
        ThermalPlant plant(20.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);

        plant.advance(-20.0f, 1.0f);

        nearlyEqual(plant.temperature(), 19.0f,
                    "Negative power must behave as zero power");
    }

    // 6. Heating and cooling can balance at an equilibrium temperature.
    {
        ThermalPlant plant(60.0f, 10.0f, 5.0f, 0.1f, 400.0f, 10.0f);

        plant.advance(100.0f, 1.0f);

        // Heating = 5; cooling = 0.1 * (60 - 10) = 5.
        nearlyEqual(plant.temperature(), 60.0f,
                    "Balanced heating and cooling must keep temperature constant");
    }

    // 7. Pressure must rise 
    {
        ThermalPlant plant(10.0f, 5.0f, 5.0f, 0.1f, 400.0f, 10.0f);

        //plant.advance(100.0f, 1.0f);

        nearlyEqual(plant.pressure(), 450.0f,
            "Pressure must rise by pressurePerDegree for each degree above ambient");
    }
    return 0;

}