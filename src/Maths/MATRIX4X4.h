#pragma once

#include "Vec4f.h"
#include "VECTOR4D.h"
#include "VECTOR3D.h"

class MATRIX4X4 {
public:
    MATRIX4X4() { LoadIdentity(); }
    MATRIX4X4(float e0, float e1, float e2, float e3,
              float e4, float e5, float e6, float e7,
              float e8, float e9, float e10, float e11,
              float e12, float e13, float e14, float e15);

    MATRIX4X4(const Sse::Vec4f v0, const Sse::Vec4f v1, const Sse::Vec4f v2, const Sse::Vec4f v3) noexcept
        : vec{v0, v1, v2, v3} { }

    MATRIX4X4(const float* rhs);
    MATRIX4X4(const MATRIX4X4& rhs);

    void SetEntry(int position, float value);
    float GetEntry(int position) const;
    VECTOR4D GetRow(int position) const;
    VECTOR4D GetColumn(int position) const;

    void LoadIdentity(void);
    void LoadZero(void);

    //binary operators
    MATRIX4X4 operator+(const MATRIX4X4& rhs) const noexcept {
        return {vec[0] + rhs.vec[0], vec[1] + rhs.vec[1], vec[2] + rhs.vec[2], vec[3] + rhs.vec[3]};
    }

    MATRIX4X4 operator-(const MATRIX4X4& rhs) const noexcept {
        return {vec[0] - rhs.vec[0], vec[1] - rhs.vec[1], vec[2] - rhs.vec[2], vec[3] - rhs.vec[3]};
    }

    MATRIX4X4 operator*(const MATRIX4X4& rhs) const noexcept {
        return {*this * rhs.vec[0], *this * rhs.vec[1], *this * rhs.vec[2], *this * rhs.vec[3]};
    }

    MATRIX4X4 operator*(const float rhs) const noexcept {
        const auto scale = Sse::Vec4f(rhs);
        return {vec[0]*scale, vec[1]*scale, vec[2]*scale, vec[3]*scale};       
    }

    MATRIX4X4 operator/(const float rhs) const noexcept {
	    if (rhs==0.0f || rhs==1.0f) return *this;
	    return *this*(1/rhs);
    }

    friend MATRIX4X4 operator*(float scaleFactor, const MATRIX4X4& rhs);

    bool operator==(const MATRIX4X4& rhs) const;
    bool operator!=(const MATRIX4X4& rhs) const;

    MATRIX4X4& operator+=(const MATRIX4X4& rhs) noexcept { return *this = *this + rhs; }
    MATRIX4X4& operator-=(const MATRIX4X4& rhs) noexcept { return *this = *this - rhs; }
    MATRIX4X4& operator*=(const MATRIX4X4& rhs) noexcept { return *this = *this * rhs; }
    MATRIX4X4& operator*=(const float rhs) noexcept { return *this = *this * rhs; }
    MATRIX4X4& operator/=(const float rhs) noexcept { return *this = *this / rhs; }

    //unary operators
    MATRIX4X4 operator-() const;
    MATRIX4X4 operator+() const { return *this; }

    //multiply a vector by this matrix

    Sse::Vec4f operator*(const Sse::Vec4f v) const noexcept {
        const auto m0 = vec[0] * v.shuffle<0, 0, 0, 0>();
        const auto m1 = vec[1] * v.shuffle<1, 1, 1, 1>();
        const auto m2 = vec[2] * v.shuffle<2, 2, 2, 2>();
        const auto m3 = vec[3] * v.shuffle<3, 3, 3, 3>();
        return m0 + m1 + m2 + m3;
    }

    VECTOR4D operator*(const VECTOR4D& rhs) const noexcept {
        VECTOR4D result;
        result.vec = *this * rhs.vec;
        return result;
    }

    //rotate a 3d vector by rotation part
    void RotateVector3D(VECTOR3D& rhs) const { rhs = GetRotatedVector3D(rhs); }

    void InverseRotateVector3D(VECTOR3D& rhs) const { rhs = GetInverseRotatedVector3D(rhs); }

    VECTOR3D GetRotatedVector3D(const VECTOR3D& rhs) const;
    VECTOR3D GetInverseRotatedVector3D(const VECTOR3D& rhs) const;

    //translate a 3d vector by translation part
    void TranslateVector3D(VECTOR3D& rhs) const { rhs = GetTranslatedVector3D(rhs); }

    void InverseTranslateVector3D(VECTOR3D& rhs) const { rhs = GetInverseTranslatedVector3D(rhs); }

    VECTOR3D GetTranslatedVector3D(const VECTOR3D& rhs) const;
    VECTOR3D GetInverseTranslatedVector3D(const VECTOR3D& rhs) const;

    //Other methods
    void Invert(void);
    MATRIX4X4 GetInverse(void) const;
    void Transpose(void);
    MATRIX4X4 GetTranspose(void) const;
    void InvertTranspose(void);
    MATRIX4X4 GetInverseTranspose(void) const;

    //Inverse of a rotation/translation only matrix
    void AffineInvert(void);
    MATRIX4X4 GetAffineInverse(void) const;
    void AffineInvertTranspose(void);
    MATRIX4X4 GetAffineInverseTranspose(void) const;

    //set to perform an operation on space - removes other entries
    void SetTranslation(const VECTOR3D& translation);
    void SetScale(const VECTOR3D& scaleFactor);
    void SetUniformScale(const float scaleFactor);
    void SetRotationAxis(const double angle, const VECTOR3D& axis);
    void SetRotationX(const double angle);
    void SetRotationY(const double angle);
    void SetRotationZ(const double angle);
    void SetRotationEuler(const double angleX, const double angleY, const double angleZ);
    void SetPerspective(float left, float right, float bottom, float top, float n, float f);
    void SetPerspective(float fovy, float aspect, float n, float f);
    void SetOrtho(float left, float right, float bottom, float top, float n, float f);

    //set parts of the matrix
    void SetTranslationPart(const VECTOR3D& translation);
    void SetRotationPartEuler(const double angleX, const double angleY, const double angleZ);

    void SetRotationPartEuler(const VECTOR3D& rotations) {
        SetRotationPartEuler(static_cast<double>(rotations.x), static_cast<double>(rotations.y), static_cast<double>(rotations.z));
    }

    //cast to pointer to a (float *) for glGetFloatv etc
    operator float*() const { return (float*)this; }
    operator const float*() const { return reinterpret_cast<const float*>(this); }

    //member variables
    union {
        float entries[16]{};
        Sse::Vec4f vec[4];
    };
};
