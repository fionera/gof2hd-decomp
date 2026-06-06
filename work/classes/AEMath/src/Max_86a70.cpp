#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float Max(float lhs, float rhs)
{
    if (lhs > rhs) {
        rhs = lhs;
    }
    return rhs;
}

} // namespace AEMath
} // namespace AbyssEngine
