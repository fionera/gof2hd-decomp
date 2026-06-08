#include "class.h"

// AbyssEngine::MODF(float, float*)
// Split value into integer (truncated toward zero) and fractional parts. The integer
// part is written through param_2; the fractional remainder is returned.
namespace AbyssEngine {

float MODF(float value, float *intPart)
{
    float ip = (float)(int)value; // VectorSignedToFloat == truncating int<->float convert
    *intPart = ip;
    return value - ip;
}

} // namespace AbyssEngine
