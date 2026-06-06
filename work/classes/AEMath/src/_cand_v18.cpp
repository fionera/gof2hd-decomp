#include "class.h"
namespace AbyssEngine { namespace AEMath {
bool operator<(const Vector &lhs, const Vector &rhs) {
    const float *base = &lhs.x;
    if (!(base[0] < rhs.x)) return false;
    if (!(base[1] < rhs.y)) return false;
    return base[2] < rhs.z;
}
} }
