#include "Vector.hpp"

#include <math.h>

float GetDirection(Vector from, Vector to)
{
    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;

    float direction = atan2(yDiff, xDiff) * 57.2957795;

    return direction;
}