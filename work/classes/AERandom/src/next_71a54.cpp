#include "class.h"

extern "C" uint32_t next_71a54(AbyssEngine::AERandom *self, int bits)
{
    uint32_t lo = ae_u32(self, 0);
    uint32_t hi = ae_u32(self, 4);
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);
    uint32_t high = newHi & 0xffffU;

    ae_u32(self, 0) = newLo;
    ae_u32(self, 4) = high;

    int shift = 0x30 - bits;
    uint32_t result = (newLo >> shift) | (high << (0x20 - shift));
    int highShift = 0x10 - bits;
    if (highShift >= 0) {
        result = high >> highShift;
    }
    return result;
}
