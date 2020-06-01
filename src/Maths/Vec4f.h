
#pragma once

#include <emmintrin.h>

namespace Sse {
    class Vec4f {
    public:
        Vec4f() noexcept = default;

        explicit Vec4f(const float f) noexcept
                : _vec(_mm_set1_ps(f)) {}

        explicit Vec4f(const float x, const float y, const float z, const float w) noexcept
                : _vec(_mm_set_ps(w, z, y, x)) {} // NOLINT

        Vec4f operator+(const Vec4f r) const noexcept { return Vec4f{_mm_add_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator-(const Vec4f r) const noexcept { return Vec4f{_mm_sub_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator*(const Vec4f r) const noexcept { return Vec4f{_mm_mul_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator/(const Vec4f r) const noexcept { return Vec4f{_mm_div_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator%(const Vec4f r) const noexcept { return *this - r * (*this / r).floor(); }

        Vec4f operator|(const Vec4f r) const noexcept { return Vec4f{_mm_and_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator&(const Vec4f r) const noexcept { return Vec4f{_mm_or_ps(_vec, r._vec)}; } // NOLINT
        Vec4f operator^(const Vec4f r) const noexcept { return Vec4f{_mm_xor_ps(_vec, r._vec)}; } // NOLINT
        // special relational operators
        Vec4f operator<(const Vec4f r) const noexcept { return Vec4f(_mm_cmplt_ps(_vec, r._vec)); }

        Vec4f operator>(const Vec4f r) const noexcept { return Vec4f(_mm_cmpgt_ps(_vec, r._vec)); }

        Vec4f operator<=(const Vec4f r) const noexcept { return Vec4f(_mm_cmple_ps(_vec, r._vec)); }

        Vec4f operator>=(const Vec4f r) const noexcept { return Vec4f(_mm_cmpge_ps(_vec, r._vec)); }
        //float operator[](const int i) const noexcept { return _vec[i]; }

        [[nodiscard]] Vec4f abs() const noexcept {
            return Vec4f{_mm_and_ps(_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)))};
        }

        [[nodiscard]] Vec4f round() const noexcept {
            auto const sgn0 = Vec4f(_mm_castsi128_ps(_mm_set1_epi32(int(0x80000000))));
            auto const or0 = (sgn0 & *this) | Vec4f(8388608.0f);
            return *this + or0 - or0;
        }

        [[nodiscard]] Vec4f floor() const noexcept {
            auto const rnd0 = round();
            return rnd0 - ((*this < rnd0) & Vec4f(1.0f));
        }

        [[nodiscard]] Vec4f ceil() const noexcept {
            auto const rnd0 = round();
            return rnd0 + ((*this > rnd0) & Vec4f(1.0f));
        }

        [[nodiscard]] Vec4f fraction() const noexcept { return *this - floor(); }

        template<unsigned int z, unsigned int y, unsigned int x, unsigned int w>
        [[nodiscard]] Vec4f shuffle() const noexcept {
            constexpr int imm = _MM_SHUFFLE(z, y, x, w); // NOLINT
            return Vec4f(_mm_shuffle_ps(_vec, _vec, imm));
        }

        [[nodiscard]] Vec4f dot(const Vec4f r) const noexcept {
            auto const mul0 = *this * r;
            auto const add0 = mul0 + mul0.shuffle<2, 3, 0, 1>();
            return add0 + add0.shuffle<0, 1, 2, 3>();
        }

        [[nodiscard]] Vec4f clamp(const Vec4f minVal, const Vec4f maxVal) const noexcept {
            return Vec4f(_mm_max_ps(_mm_min_ps(_vec, maxVal._vec), minVal._vec)); // NOLINT
        }

        [[nodiscard]] Vec4f sqrt() const noexcept { return Vec4f(_mm_sqrt_ps(_vec)); }

        [[nodiscard]] Vec4f r_sqrt() const noexcept { return Vec4f(_mm_rsqrt_ps(_vec)); }

        [[nodiscard]] Vec4f lengthSqr() const noexcept { return dot(*this); }

        [[nodiscard]] Vec4f length() const noexcept { return lengthSqr().sqrt(); }

        [[nodiscard]] Vec4f normalized() const noexcept { return *this * lengthSqr().r_sqrt(); }

        [[nodiscard]] int sign_bits() const noexcept { return _mm_movemask_ps(_vec); }

        [[nodiscard]] Vec4f xyzw() const noexcept { return shuffle<3, 2, 1, 0>(); }

        static Vec4f nan() noexcept { return Vec4f(_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))); }

        static Vec4f inf() noexcept { return Vec4f(_mm_castsi128_ps(_mm_set1_epi32(0x7F800000))); }

    private:
        explicit Vec4f(__m128 v) noexcept
                : _vec(v) {}

        __m128 _vec;
    };

    inline Vec4f fma(const Vec4f a, const Vec4f b, const Vec4f c) noexcept { return a * b + c; }

    inline Vec4f sign(const Vec4f x) noexcept {
        auto const zro0 = Vec4f(0.0f);
        return ((x < zro0) & Vec4f(-1.0f)) | ((x > zro0) & Vec4f(1.0f));
    }

    inline Vec4f mix(const Vec4f v1, const Vec4f v2, const Vec4f a) noexcept {
        return fma(v2, a, v1 * (Vec4f(1.0f) - a));
    }

    inline Vec4f step(const Vec4f edge, const Vec4f x) noexcept {
        return !(x <= edge).sign_bits() ? Vec4f(1.0f) : Vec4f(0.0f);
    }

    inline Vec4f smooth_step(const Vec4f edge0, const Vec4f edge1, const Vec4f x) noexcept {
        auto const clp0 = ((x - edge0) / (edge1 - edge0)).clamp(Vec4f(0.0), Vec4f(1.0f));
        auto const mul1 = clp0 * clp0;
        return mul1 * (Vec4f(3.0f) - clp0 * Vec4f(2.0f));
    }

    inline Vec4f distance(const Vec4f p0, const Vec4f p1) noexcept { return (p0 - p1).length(); }

    inline Vec4f cross(const Vec4f v1, const Vec4f v2) noexcept {
        return v1.shuffle<3, 0, 2, 1>() * v2.shuffle<3, 1, 0, 2>() -
               v1.shuffle<3, 1, 0, 2>() * v2.shuffle<3, 0, 2, 1>();
    }

    inline Vec4f face_forward(const Vec4f N, const Vec4f I, const Vec4f Nref) noexcept {
        return N * (sign(Nref.dot(I)) * Vec4f(-1.0f));
    }

    inline Vec4f reflect(const Vec4f I, const Vec4f N) noexcept { return I - N * N.dot(I) * Vec4f(2.0f); }

    inline Vec4f refract(const Vec4f I, const Vec4f N, const Vec4f eta) noexcept {
        auto const dot0 = N.dot(I);
        auto const mul2 = (Vec4f(1.0f) - eta * eta) * (Vec4f(1.0f) - dot0 * dot0);
        if (!(mul2 > Vec4f(0.0f)).sign_bits()) return Vec4f(0.0f);
        return eta * I * (fma(eta, dot0, mul2.sqrt()) * N);
    }
}