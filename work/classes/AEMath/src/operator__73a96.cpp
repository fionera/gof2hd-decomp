#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator+(const Vector &value)
{
    Vector result;
    double xy = *reinterpret_cast<const double *>(&value.x);
    float z = value.z;
    result.z = z;
    *reinterpret_cast<double *>(&result.x) = xy;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
