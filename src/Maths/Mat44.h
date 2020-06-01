#pragma once

#include "Vec4f.h"
#include "Vec4.h"
#include "Vec3.h"

class Mat44 {
public:
    Mat44() { LoadIdentity(); }

    Mat44(float e0, float e1, float e2, float e3,
          float e4, float e5, float e6, float e7,
          float e8, float e9, float e10, float e11,
          float e12, float e13, float e14, float e15);

    Mat44(const Sse::Vec4f v0, const Sse::Vec4f v1, const Sse::Vec4f v2, const Sse::Vec4f v3) noexcept
            : vec{v0, v1, v2, v3} {}

    Mat44(const float *rhs);

    Mat44(const Mat44 &rhs);

    void SetEntry(int position, float value);

    float GetEntry(int position) const;

    Vec4 GetRow(int position) const;

    Vec4 GetColumn(int position) const;

    void LoadIdentity(void);

    void LoadZero(void);

    //binary operators
    Mat44 operator+(const Mat44 &rhs) const noexcept {
        return {vec[0] + rhs.vec[0], vec[1] + rhs.vec[1], vec[2] + rhs.vec[2], vec[3] + rhs.vec[3]};
    }

    Mat44 operator-(const Mat44 &rhs) const noexcept {
        return {vec[0] - rhs.vec[0], vec[1] - rhs.vec[1], vec[2] - rhs.vec[2], vec[3] - rhs.vec[3]};
    }

    Mat44 operator*(const Mat44 &rhs) const noexcept {
        return {*this * rhs.vec[0], *this * rhs.vec[1], *this * rhs.vec[2], *this * rhs.vec[3]};
    }

    Mat44 operator*(const float rhs) const noexcept {
        const auto scale = Sse::Vec4f(rhs);
        return {vec[0] * scale, vec[1] * scale, vec[2] * scale, vec[3] * scale};
    }

    Mat44 operator/(const float rhs) const noexcept {
        if (rhs == 0.0f || rhs == 1.0f) return *this;
        return *this * (1 / rhs);
    }

    Mat44 &operator+=(const Mat44 &rhs) noexcept { return *this = *this + rhs; }

    Mat44 &operator-=(const Mat44 &rhs) noexcept { return *this = *this - rhs; }

    Mat44 &operator*=(const Mat44 &rhs) noexcept { return *this = *this * rhs; }

    Mat44 &operator*=(const float rhs) noexcept { return *this = *this * rhs; }

    Mat44 &operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    friend Mat44 operator*(float scaleFactor, const Mat44 &rhs);

    bool operator==(const Mat44 &rhs) const;

    bool operator!=(const Mat44 &rhs) const;

    //unary operators
    Mat44 operator-() const;

    Mat44 operator+() const { return *this; }

    //multiply a vector by this matrix
    Sse::Vec4f operator*(const Sse::Vec4f v) const noexcept {
        const auto m0 = vec[0] * v.shuffle<0, 0, 0, 0>();
        const auto m1 = vec[1] * v.shuffle<1, 1, 1, 1>();
        const auto m2 = vec[2] * v.shuffle<2, 2, 2, 2>();
        const auto m3 = vec[3] * v.shuffle<3, 3, 3, 3>();
        return m0 + m1 + m2 + m3;
    }

    Vec4 operator*(const Vec4 &rhs) const noexcept {
        Vec4 result;
        result.vec = *this * rhs.vec;
        return result;
    }

    //Other methods
    void Invert();

    Mat44 GetInverse() const;

    void Transpose();

    Mat44 GetTranspose() const;

    void InvertTranspose();

    Mat44 GetInverseTranspose() const;

    //Inverse of a rotation/translation only matrix
    void AffineInvert();

    Mat44 GetAffineInverse() const;

    void AffineInvertTranspose();

    Mat44 GetAffineInverseTranspose() const;

    //set to perform an operation on space - removes other entries
    void SetTranslation(const Vec3 &translation);

    void SetScale(const Vec3 &scaleFactor);

    void SetUniformScale(const float scaleFactor);

    void SetRotationAxis(const double angle, const Vec3 &axis);

    void SetRotationX(const double angle);

    void SetRotationY(const double angle);

    void SetRotationZ(const double angle);

    void SetRotationEuler(const double angleX, const double angleY, const double angleZ);

    void SetPerspective(float left, float right, float bottom, float top, float n, float f);

    void SetPerspective(float fovy, float aspect, float n, float f);

    void SetOrtho(float left, float right, float bottom, float top, float n, float f);

    //set parts of the matrix
    void SetTranslationPart(const Vec3 &translation);

    void SetRotationPartEuler(const double angleX, const double angleY, const double angleZ);

    void SetRotationPartEuler(const Vec3 &rotations) {
        SetRotationPartEuler(static_cast<double>(rotations.x), static_cast<double>(rotations.y),
                             static_cast<double>(rotations.z));
    }

    //member variables
    union {
        float entries[16]{};
        Sse::Vec4f vec[4];
    };
};
