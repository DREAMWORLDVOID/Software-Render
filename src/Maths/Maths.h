#pragma once

#include <cmath>

//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//An epsilon value for comparisons
#ifndef EPSILON
#define EPSILON 0.01f
#endif

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat44.h"

template<class T>
inline T QuadraticInterpolate(const T &v1, const T &v2, const T &v3, float factor) noexcept {
    const auto fctInv = 1.0f - factor;
    return v1 * fctInv * fctInv + 2 * v2 * factor * fctInv + v3 * factor * factor;
}

template<class T>
inline T lerp(const T &v1, const T &v2, float factor) noexcept { return v1 * (1.0f - factor) + v2 * factor; }
