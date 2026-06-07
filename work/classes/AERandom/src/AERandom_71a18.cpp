#include "class.h"

extern "C" void AERandom_71a18(AbyssEngine::AERandom *self, long long seed)
{
    ae_u32(self, 0) = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    ae_u32(self, 4) = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

