#include "class.h"

// Globals::sqrt(float) - the target moves the float arg (r1) to r0 and tail-jumps to the
// runtime sqrtf. The instance pointer (r0) is unused; the float arrives in r1.
extern "C" float Globals_sqrt_impl(float x);

extern "C" float Globals_sqrt(Globals *self, float x)
{
    (void)self;
    return Globals_sqrt_impl(x);
}
