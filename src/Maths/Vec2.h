#pragma once

class Vec2 {
public:
    //constructors
    Vec2() : x(0.0f), y(0.0f) {}

    Vec2(float newX, float newY) : x(newX), y(newY) {}

    Vec2(const Vec2 &rhs) : x(rhs.x), y(rhs.y) {}

    //Accessors kept for compatibility
    void SetX(float newX) { x = newX; }

    void SetY(float newY) { y = newY; }

    [[nodiscard]] float GetX() const { return x; }

    [[nodiscard]] float GetY() const { return y; }

    [[nodiscard]] float GetLength() const noexcept { return sqrt(GetSquaredLength()); }

    [[nodiscard]] float GetSquaredLength() const noexcept { return (x * x) + (y * y); }

    void Normalize() noexcept {
        const auto length = GetSquaredLength();
        if (length == 1 || length == 0) return;
        *this *= (1.0f / sqrt(length));
    }

    [[nodiscard]] Vec2 GetNormalized() const noexcept {
        auto result = *this;
        result.Normalize();
        return result;
    }

    //overloaded operators
    //binary operators
    Vec2 operator+(const Vec2 &rhs) const { return Vec2(x + rhs.x, y + rhs.y); }

    Vec2 operator-(const Vec2 &rhs) const { return Vec2(x - rhs.x, y - rhs.y); }

    Vec2 operator*(const float rhs) const { return Vec2(x * rhs, y * rhs); }

    Vec2 operator/(const float rhs) const { return (rhs == 0) ? Vec2(0.0f, 0.0f) : Vec2(x / rhs, y / rhs); }

    //self-add etc
    Vec2 &operator+=(const Vec2 &rhs) noexcept { return *this = *this + rhs; }

    Vec2 &operator-=(const Vec2 &rhs) noexcept { return *this = *this - rhs; }

    Vec2 &operator*=(const float rhs) noexcept { return *this = *this * rhs; }

    Vec2 &operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    bool operator==(const Vec2 &rhs) const noexcept { return x == rhs.x && y == rhs.y; }

    bool operator!=(const Vec2 &rhs) const noexcept { return !((*this) == rhs); }

    //unary operators
    Vec2 operator-() const { return Vec2(-x, -y); }

    Vec2 operator+() const { return *this; }

    //member variables
    float x;
    float y;
};

inline Vec2 operator*(float scale, const Vec2 &rhs) noexcept { return rhs * scale; }

