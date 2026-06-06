#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector::operator const float *() const
{
    return &x;
}

} // namespace AEMath
} // namespace AbyssEngine
