#include "Maths.h"

bool Vec4::operator==(const Vec4 &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

Vec4::operator Vec3() const {
    if (w == 0.0f || w == 1.0f)
        return Vec3(x, y, z);
    else
        return Vec3(x / w, y / w, z / w);
}
