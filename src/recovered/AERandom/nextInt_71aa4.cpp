#include "class.h"

extern "C" uint32_t nextInt_71aa4(AbyssEngine::AERandom *self)
{
    uint32_t lo = ae_u32(self, 0);
    uint32_t hi = ae_u32(self, 4);
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    ae_u32(self, 0) = newLo;
    ae_u32(self, 4) = newHi & 0xffffU;
    return (newLo >> 16) | (newHi << 16);
}
