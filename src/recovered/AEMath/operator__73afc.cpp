#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator-(const Vector &lhs, const Vector &rhs)
{
    Vector result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
