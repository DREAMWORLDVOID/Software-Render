#pragma once

class Vec3 {
public:
    //constructors
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    Vec3(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ) {}

    Vec3(const Vec3 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

    //Accessors kept for compatibility
    void SetX(float newX) { x = newX; }

    void SetY(float newY) { y = newY; }

    void SetZ(float newZ) { z = newZ; }

    [[nodiscard]] float GetX() const { return x; }

    [[nodiscard]] float GetY() const { return y; }

    [[nodiscard]] float GetZ() const { return z; }

    [[nodiscard]] float GetSquaredLength() const { return (x * x) + (y * y) + (z * z); }

    [[nodiscard]] float GetLength() const { return sqrt(GetSquaredLength()); }

    void Normalize() noexcept {
        const auto length = GetSquaredLength();
        if (length == 1 || length == 0) return;
        *this *= (1.0f / sqrt(length));
    }

    [[nodiscard]] Vec3 GetNormalized() const noexcept {
        auto result = *this;
        result.Normalize();
        return result;
    }

    //vector algebra
    [[nodiscard]] Vec3 CrossProduct(const Vec3 &rhs) const {
        return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }

    [[nodiscard]] float DotProduct(const Vec3 &rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

    //overloaded operators
    //binary operators
    Vec3 operator+(const Vec3 &rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }

    Vec3 operator-(const Vec3 &rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }

    Vec3 operator*(const float rhs) const { return Vec3(x * rhs, y * rhs, z * rhs); }

    Vec3 operator/(const float rhs) const {
        return (rhs == 0.0f) ? Vec3(0.0f, 0.0f, 0.0f) : Vec3(x / rhs, y / rhs, z / rhs);
    }

    //self-add etc
    Vec3 &operator+=(const Vec3 &rhs) noexcept { return *this = *this + rhs; }

    Vec3 &operator-=(const Vec3 &rhs) noexcept { return *this = *this - rhs; }

    Vec3 &operator*=(const float rhs) noexcept { return *this = *this * rhs; }

    Vec3 &operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    bool operator==(const Vec3 &rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }

    bool operator!=(const Vec3 &rhs) const noexcept { return !((*this) == rhs); }

    //unary operators
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3 operator+() const { return *this; }

    //member variables
    float x;
    float y;
    float z;
};

inline Vec3 operator*(float scale, const Vec3 &rhs) noexcept { return rhs * scale; }
