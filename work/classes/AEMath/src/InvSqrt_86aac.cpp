#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float InvSqrt(float value)
{
    static const int magic = 0x5f3759df;
    union {
        float f;
        int i;
    } bits;

    bits.f = value;
    bits.i = magic - (bits.i >> 1);
    value = value * -0.5f * bits.f * bits.f + 1.5f;
    return value * bits.f;
}

} // namespace AEMath
} // namespace AbyssEngine
