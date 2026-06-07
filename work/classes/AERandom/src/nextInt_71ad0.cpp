#include "class.h"

static inline __attribute__((always_inline)) uint32_t ae_next31(AbyssEngine::AERandom *self)
{
    uint32_t lo = ae_u32(self, 0);
    uint32_t hi = ae_u32(self, 4);
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    ae_u32(self, 0) = newLo;
    ae_u32(self, 4) = newHi & 0xffffU;
    return (newLo >> 17) | (newHi << 15);
}

extern "C" int nextInt_71ad0(AbyssEngine::AERandom *self, int bound)
{
    int mask = bound - 1;
    if ((bound & -bound) == bound) {
        return static_cast<int>((static_cast<int64_t>(bound) * ae_next31(self)) >> 31);
    }

    int bits;
    int value;
    do {
        bits = static_cast<int>(ae_next31(self));
        value = bits % bound;
    } while (bits - value + mask < 0);
    return value;
}
