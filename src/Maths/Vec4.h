#pragma once

#include "Vec4f.h"
#include "Vec3.h"

class Vec4 {
public:
    //constructors
    Vec4() noexcept : vec(0.0f) {}

    Vec4(float newX, float newY, float newZ, float newW) noexcept: vec(newX, newY, newZ, newW) {}

    Vec4(const Vec4 &rhs) noexcept: vec(rhs.vec) {}

    explicit Vec4(const Sse::Vec4f v) noexcept: vec(v) {}

    explicit Vec4(const Vec3 &rhs) noexcept : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f) {}

    void SetX(const float newX) noexcept { x = newX; }

    void SetY(const float newY) noexcept { y = newY; }

    void SetZ(const float newZ) noexcept { z = newZ; }

    void SetW(const float newW) noexcept { w = newW; }

    [[nodiscard]] float GetX() const noexcept { return x; }

    [[nodiscard]] float GetY() const noexcept { return y; }

    [[nodiscard]] float GetZ() const noexcept { return z; }

    [[nodiscard]] float GetW() const noexcept { return w; }

    //vector algebra
    float DotProduct(const Vec4 &rhs) { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

    //binary operators
    Vec4 operator+(const Vec4 &rhs) const noexcept { return Vec4(vec + rhs.vec); }

    Vec4 operator-(const Vec4 &rhs) const noexcept { return Vec4(vec - rhs.vec); }

    Vec4 operator*(const Vec4 &rhs) const noexcept { return Vec4(vec * rhs.vec); }

    Vec4 operator*(const float rhs) const noexcept { return Vec4(vec * Sse::Vec4f{rhs}); }

    Vec4 operator/(const float rhs) const noexcept {
        return Vec4(rhs == 0.0f ? Sse::Vec4f{0.0f} : vec / Sse::Vec4f{rhs});
    }

    //self-add etc
    Vec4 &operator+=(const Vec4 &rhs) noexcept { return *this = *this + rhs; }

    Vec4 &operator-=(const Vec4 &rhs) noexcept { return *this = *this - rhs; }

    Vec4 &operator*=(const Vec4 &rhs) noexcept { return *this = *this * rhs; }

    Vec4 &operator*=(const float rhs) noexcept { return *this = *this * rhs; }

    Vec4 &operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    bool operator==(const Vec4 &rhs) const;

    bool operator!=(const Vec4 &rhs) const { return !((*this) == rhs); }

    //unary operators
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

    Vec4 operator+() const { return (*this); }

    [[nodiscard]] auto Trim() const noexcept { return Vec3(x, y, z); }

    [[nodiscard]] auto Sse() const noexcept { return vec; }

    //member variables
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };

        Sse::Vec4f vec;
    };

};

inline Vec4 operator*(float scale, const Vec4 &rhs) noexcept { return rhs * scale; }
