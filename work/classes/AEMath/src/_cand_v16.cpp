#include "class.h"
namespace AbyssEngine { namespace AEMath {
bool operator<(const Vector &lhs, const Vector &rhs) {
    const Vector *p = &lhs;
    if (!(lhs.x < rhs.x)) return false;
    if (!(p->y < rhs.y)) return false;
    return p->z < rhs.z;
}
} }
