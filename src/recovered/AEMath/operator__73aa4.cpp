#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator-(const Vector &value)
{
    Vector result;
    result.x = -value.x;
    result.y = -value.y;
    result.z = -value.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
