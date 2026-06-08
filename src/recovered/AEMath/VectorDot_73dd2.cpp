#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float VectorDot(const Vector &lhs, const Vector &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine
