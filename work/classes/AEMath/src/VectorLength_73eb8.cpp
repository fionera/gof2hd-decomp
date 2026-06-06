#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float VectorLength(const Vector &value)
{
    float length = value.y * value.y;
    length += value.x * value.x;
    length += value.z * value.z;
    return sqrtf(length);
}

} // namespace AEMath
} // namespace AbyssEngine
