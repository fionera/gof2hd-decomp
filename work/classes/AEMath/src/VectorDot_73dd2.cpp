#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float VectorDot(const Vector &lhs, const Vector &rhs)
{
    float result = lhs.y * rhs.y;
    result += lhs.x * rhs.x;
    result += lhs.z * rhs.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
