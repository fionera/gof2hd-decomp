#include "quaternion.h"

extern "C" const float *operator_cast_to_float_814d6(const AbyssEngine::Quaternion *self)
    __asm__("operator.cast.to.float*");

extern "C" const float *operator_cast_to_float_814d6(const AbyssEngine::Quaternion *self)
{
    return &self->x;
}
