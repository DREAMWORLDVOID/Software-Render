#include "Maths.h"

bool Vec4::operator==(const Vec4 &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}
