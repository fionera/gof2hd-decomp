#include "class.h"

namespace AbyssEngine {
namespace AEMath {

bool operator>(const Vector &lhs, const Vector &rhs)
{
    const volatile Vector *left = &lhs;
    bool result = false;
    if (!(left->x > rhs.x)) {
        return false;
    }
    if (left->y > rhs.y) {
        result = left->z > rhs.z;
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
