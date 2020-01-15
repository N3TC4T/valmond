#include "utils.hpp"

#include "iostream"
#include <cmath>

float
floatDecimalPoint(float number)
{
    return std::floor(number * 100.0) / 100.0;
}
