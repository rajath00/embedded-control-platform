#pragma once

#include <cassert>
#include <cmath>
#include <iostream>

constexpr float EPSILON = 0.001f;

void nearlyEqual(float actual, float expected,const char* message)
{   
    if (std::fabs(actual - expected) >= EPSILON)
    {
        std::cerr << "Test failed: " << message << '\n'
            << "  Expected: " << expected << '\n'
            << "  Actual:   " << actual << '\n';
    }
    assert(std::fabs(actual - expected) < EPSILON);
}