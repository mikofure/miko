#include "miko/utils/Math.h"
#include <algorithm>
#include <cmath>

namespace miko {



// Size implementations









// Margin implementations

// Utility functions

float InverseLerp(float a, float b, float value) {
    if (a == b) return 0;
    return (value - a) / (b - a);
}

float Remap(float value, float fromMin, float fromMax, float toMin, float toMax) {
    float t = InverseLerp(fromMin, fromMax, value);
    return Lerp(toMin, toMax, t);
}

bool NearlyEqual(float a, float b, float epsilon) {
    return std::abs(a - b) <= epsilon;
}

float DegreesToRadians(float degrees) {
    return degrees * (3.14159265359f / 180.0f);
}

float RadiansToDegrees(float radians) {
    return radians * (180.0f / 3.14159265359f);
}

} // namespace miko