
#pragma once

#include <smmintrin.h>
#include <cstdint>

namespace Sse {
    class alignas(16) Vec4I {
    public:
        Vec4I() noexcept = default;

        explicit Vec4I(const int32_t f) noexcept
                : _vec(_mm_set1_epi32(f)) {}

        explicit Vec4I(const int32_t z, const int32_t y, const int32_t x, const int32_t w) noexcept
                : _vec(_mm_set_epi32(z, y, x, w)) {}

        Vec4I operator+(const Vec4I r) const noexcept { return Vec4I{_mm_add_epi32(_vec, r._vec)}; } // NOLINT
        Vec4I operator-(const Vec4I r) const noexcept { return Vec4I{_mm_sub_epi32(_vec, r._vec)}; } // NOLINT
        Vec4I operator*(const Vec4I r) const noexcept { return Vec4I{_mm_mul_epi32(_vec, r._vec)}; } // NOLINT
        Vec4I operator|(const Vec4I r) const noexcept { return Vec4I{_mm_and_si128(_vec, r._vec)}; } // NOLINT
        Vec4I operator&(const Vec4I r) const noexcept { return Vec4I{_mm_or_si128(_vec, r._vec)}; } // NOLINT
        Vec4I operator^(const Vec4I r) const noexcept { return Vec4I{_mm_xor_si128(_vec, r._vec)}; } // NOLINT
        // special relational operators
        Vec4I operator<(const Vec4I r) const noexcept { return Vec4I(_mm_cmplt_epi32(_vec, r._vec)); }

        Vec4I operator>(const Vec4I r) const noexcept { return Vec4I(_mm_cmpgt_epi32(_vec, r._vec)); }

        Vec4I operator<=(const Vec4I r) const noexcept { return Vec4I(_mm_cmpgt_epi32(r._vec, _vec)); }

        Vec4I operator>=(const Vec4I r) const noexcept { return Vec4I(_mm_cmpgt_epi32(r._vec, _vec)); }

        int32_t operator[](const int i) const noexcept { return _vec[i]; }

        template<unsigned int z, unsigned int y, unsigned int x, unsigned int w>
        [[nodiscard]] Vec4I shuffle() const noexcept {
            constexpr int imm = _MM_SHUFFLE(z, y, x, w); // NOLINT
            return Vec4I(_mm_shuffle_epi32(_vec, imm));
        }

        [[nodiscard]] Vec4I dot(const Vec4I r) const noexcept {
            auto const mul0 = *this * r;
            auto const add0 = mul0 + mul0.shuffle<2, 3, 0, 1>();
            return add0 + add0.shuffle<0, 1, 2, 3>();
        }

        [[nodiscard]] Vec4I clamp(const Vec4I minVal, const Vec4I maxVal) const noexcept {
            return Vec4I(_mm_max_epi32(_mm_min_epi32(_vec, maxVal._vec), minVal._vec)); // NOLINT
        }

        [[nodiscard]] Vec4I lengthSqr() const noexcept { return dot(*this); }

        [[nodiscard]] Vec4I xyzw() const noexcept { return shuffle<3, 2, 1, 0>(); }

    private:
        explicit Vec4I(__m128i v) noexcept
                : _vec(v) {}

        __v4si _vec;
    };

    inline Vec4I fma(const Vec4I a, const Vec4I b, const Vec4I c) noexcept { return a * b + c; }

    inline Vec4I cross(const Vec4I v1, const Vec4I v2) noexcept {
        return v1.shuffle<3, 0, 2, 1>() * v2.shuffle<3, 1, 0, 2>() -
               v1.shuffle<3, 1, 0, 2>() * v2.shuffle<3, 0, 2, 1>();
    }
}