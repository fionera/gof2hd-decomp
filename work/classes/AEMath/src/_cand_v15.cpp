#include "class.h"
namespace AbyssEngine { namespace AEMath {
bool operator<(const Vector &lhs, const Vector &rhs) {
    const float *ly = &lhs.y;
    if (!(lhs.x < rhs.x)) return false;
    if (!(ly[0] < rhs.y)) return false;
    return ly[1] < rhs.z;
}
} }
