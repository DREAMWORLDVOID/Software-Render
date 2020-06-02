#pragma once

#include <cmath>
#include "Vec4f.h"

class Vec3 {
public:
    //constructors
    Vec3() : vec(0.0) {}

    Vec3(const Sse::Vec4f v) noexcept: vec(v) {}

    Vec3(float newX, float newY, float newZ) : vec(newX, newY, newZ, 0) {}

    Vec3(const Vec3 &rhs) : vec(rhs.vec) {}

    //Accessors kept for compatibility
    void SetX(float newX) { x = newX; }

    void SetY(float newY) { y = newY; }

    void SetZ(float newZ) { z = newZ; }

    [[nodiscard]] float GetX() const { return x; }

    [[nodiscard]] float GetY() const { return y; }

    [[nodiscard]] float GetZ() const { return z; }

    [[nodiscard]] float GetLength() const { return Vec3{vec.length()}.GetX(); }

    [[nodiscard]] float GetSquaredLength() const { return Vec3{vec.lengthSqr()}.GetX(); }

    void Normalize() noexcept { vec = vec.normalized(); }

    [[nodiscard]] Vec3 GetNormalized() const noexcept { return Vec3(vec.normalized()); }

    //vector algebra
    [[nodiscard]] Vec3 CrossProduct(const Vec3 &rhs) const {
        return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }

    [[nodiscard]] float DotProduct(const Vec3 &rhs) const { return Vec3(vec.dot(rhs.vec)).GetX(); }

    //overloaded operators
    //binary operators
    Vec3 operator+(const Vec3 &rhs) const { return Vec3(vec + rhs.vec); }

    Vec3 operator-(const Vec3 &rhs) const { return Vec3(vec - rhs.vec);}

    Vec3 operator*(const Vec3 &rhs) const { return Vec3(vec * rhs.vec);}

    Vec3 operator*(const float rhs) const { return Vec3(vec * Sse::Vec4f(rhs)); }

    Vec3 operator/(const float rhs) const {
        return (rhs == 0.0f) ? Vec3(0.0f, 0.0f, 0.0f) : Vec3(x / rhs, y / rhs, z / rhs);
    }

    //self-add etc
    Vec3 &operator+=(const Vec3 &rhs) noexcept { return *this = *this + rhs; }

    Vec3 &operator-=(const Vec3 &rhs) noexcept { return *this = *this - rhs; }

    Vec3 &operator*=(const Vec3 &rhs) noexcept { return *this = *this * rhs; }

    Vec3 &operator*=(const float rhs) noexcept { return *this = *this * rhs; }

    Vec3 &operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    bool operator==(const Vec3 &rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }

    bool operator!=(const Vec3 &rhs) const noexcept { return !((*this) == rhs); }

    //unary operators
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3 operator+() const { return *this; }

    [[nodiscard]] auto Sse() const noexcept { return vec; }

    //member variables
    union {
        struct {
            float x;
            float y;
            float z;
        };
        Sse::Vec4f vec;
    };
};

inline Vec3 operator*(float scale, const Vec3 &rhs) noexcept { return rhs * scale; }
