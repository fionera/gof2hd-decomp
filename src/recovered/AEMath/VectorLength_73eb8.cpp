#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float VectorLength(const Vector &value)
{
    return sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
}

} // namespace AEMath
} // namespace AbyssEngine
