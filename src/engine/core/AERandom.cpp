#include "gof2/AERandom.h"
#include <ctime>

// ---- AERandom_719e0.cpp ----
AbyssEngine::AERandom *AERandom_719e0(AbyssEngine::AERandom *self)
{
    int seed = time(0);
    self->seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->seedHigh = (static_cast<uint32_t>(seed >> 31) & 0xffffU) ^ 5U;
    return self;
}

// ---- nextInt_71aa4.cpp ----
uint32_t nextInt_71aa4(AbyssEngine::AERandom *self)
{
    uint32_t lo = self->seedLow;
    uint32_t hi = self->seedHigh;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    self->seedLow = newLo;
    self->seedHigh = newHi & 0xffffU;
    return (newLo >> 16) | (newHi << 16);
}

// ---- AERandom_71a18.cpp ----
void AERandom_71a18(AbyssEngine::AERandom *self, long long seed)
{
    self->seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->seedHigh = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

// ---- setSeed_71a04.cpp ----
void setSeed_71a04(AbyssEngine::AERandom *self, long long seed)
{
    self->seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->seedHigh = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

// ---- reset_71a30.cpp ----
void reset_71a30(AbyssEngine::AERandom *self)
{
    int seed = time(0);
    self->seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->seedHigh = (static_cast<uint32_t>(seed >> 31) & 0xffffU) ^ 5U;
}

// ---- next_71a54.cpp ----
uint32_t next_71a54(AbyssEngine::AERandom *self, int bits)
{
    uint32_t lo = self->seedLow;
    uint32_t hi = self->seedHigh;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);
    uint32_t high = newHi & 0xffffU;

    self->seedLow = newLo;
    self->seedHigh = high;

    int shift = 0x30 - bits;
    uint32_t result = (newLo >> shift) | (high << (0x20 - shift));
    int highShift = 0x10 - bits;
    if (highShift >= 0) {
        result = high >> highShift;
    }
    return result;
}

// ---- nextInt_71ad0.cpp ----
static inline __attribute__((always_inline)) uint32_t ae_next31(AbyssEngine::AERandom *self)
{
    uint32_t lo = self->seedLow;
    uint32_t hi = self->seedHigh;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    self->seedLow = newLo;
    self->seedHigh = newHi & 0xffffU;
    return (newLo >> 17) | (newHi << 15);
}

int nextInt_71ad0(AbyssEngine::AERandom *self, int bound)
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

// ============================================================================
// Real AERandom methods (recovered: 0x819e0..0x81ad0). The free functions
// above are the legacy extern "C" bodies; these mirror their semantics on the
// class itself.
// ============================================================================
namespace AbyssEngine {

AERandom::AERandom()
{
    reset();
}

AERandom::AERandom(long long seed)
{
    setSeed(seed);
}

AERandom::~AERandom()
{
}

void AERandom::setSeed(long long seed)
{
    seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    seedHigh = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

void AERandom::reset()
{
    long long now = time(0);
    seedLow = 0xdeece66dU ^ static_cast<uint32_t>(now);
    seedHigh = (static_cast<uint32_t>(now >> 31) & 0xffffU) ^ 5U;
}

uint32_t AERandom::next(int bits)
{
    uint64_t seed = (static_cast<uint64_t>(seedHigh) << 32) | seedLow;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32) & 0xffffU;

    seedLow = newLo;
    seedHigh = newHi;

    return static_cast<uint32_t>(((static_cast<uint64_t>(newHi) << 32) | newLo) >> (48 - bits));
}

int AERandom::nextInt()
{
    return static_cast<int>(next(31));
}

int AERandom::nextInt(int bound)
{
    if ((bound & -bound) == bound) {
        // power of two: use high bits directly
        return static_cast<int>((static_cast<int64_t>(bound) * next(31)) >> 31);
    }

    int bits;
    int value;
    do {
        bits = static_cast<int>(next(31));
        value = bits % bound;
    } while (bits - value + (bound - 1) < 0);
    return value;
}

} // namespace AbyssEngine
