#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float Absf(float value)
{
    if (!(value >= 0.0f)) {
        value = -value;
    }
    return value;
}

} // namespace AEMath
} // namespace AbyssEngine
