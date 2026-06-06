#include "class.h"

namespace AbyssEngine {
namespace AEMath {

bool operator>=(const Vector &lhs, const Vector &rhs)
{
    if (!(lhs.x >= rhs.x)) {
        return false;
    }
    bool result = false;
    if (!(lhs.y >= rhs.y)) {
        return result;
    }
    if (lhs.z >= rhs.z) {
        result = true;
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
