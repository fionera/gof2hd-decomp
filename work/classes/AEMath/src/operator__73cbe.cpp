#include "class.h"

namespace AbyssEngine {
namespace AEMath {

bool operator<(const Vector &lhs, const Vector &rhs)
{
    if (!(lhs.x < rhs.x)) {
        return false;
    }
    if (!(lhs.y < rhs.y)) {
        return false;
    }
    return lhs.z < rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine
