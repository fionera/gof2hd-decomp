#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator/=(float scale)
{
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
