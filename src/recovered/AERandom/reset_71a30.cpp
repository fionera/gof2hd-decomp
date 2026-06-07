#include "class.h"

extern "C" int time(int *);

extern "C" void reset_71a30(AbyssEngine::AERandom *self)
{
    int seed = time(0);
    ae_u32(self, 0) = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    ae_u32(self, 4) = (static_cast<uint32_t>(seed >> 31) & 0xffffU) ^ 5U;
}

