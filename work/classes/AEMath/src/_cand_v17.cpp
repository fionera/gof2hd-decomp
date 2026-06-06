#include "class.h"
namespace AbyssEngine { namespace AEMath {
bool operator<(const Vector &lhs, const Vector &rhs) {
    const Vector *p = &lhs;
    const Vector *q = &rhs;
    if (!(lhs.x < q->x)) return false;
    if (!(p->y < q->y)) return false;
    return p->z < q->z;
}
} }
