#include "class.h"

extern "C" int AERandom_nextInt(int rng, int bound);
// createChar(bool,int) — 3 register args in practice (r0=clz bool, r1=rand==0, r2=type).
extern "C" void ImageFactory_createChar_bi(int a, int b, int type);
// PC-relative pointer-to-pointer global: *g -> rng object.
extern void *const gCreateCharRng __attribute__((visibility("hidden")));

// ImageFactory::createChar(int) — randomizes a char then forwards to createChar(bool,int).
extern "C" void ImageFactory_createChar_i(ImageFactory *self, int param_1)
{
    (void)self;
    int r = AERandom_nextInt(*(int *)gCreateCharRng, 2);
    ImageFactory_createChar_bi((int)__builtin_clz(r), (unsigned)(r == 0), param_1);
}
