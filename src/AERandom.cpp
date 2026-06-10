#include "gof2/AERandom.h"
#include <ctime>

// ---- AERandom_719e0.cpp ----
AbyssEngine::AERandom *AERandom_719e0(AbyssEngine::AERandom *self)
{
    int seed = time(0);
    self->field_0x0 = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->field_0x4 = (static_cast<uint32_t>(seed >> 31) & 0xffffU) ^ 5U;
    return self;
}

// ---- nextInt_71aa4.cpp ----
uint32_t nextInt_71aa4(AbyssEngine::AERandom *self)
{
    uint32_t lo = self->field_0x0;
    uint32_t hi = self->field_0x4;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    self->field_0x0 = newLo;
    self->field_0x4 = newHi & 0xffffU;
    return (newLo >> 16) | (newHi << 16);
}

// ---- AERandom_71a18.cpp ----
void AERandom_71a18(AbyssEngine::AERandom *self, long long seed)
{
    self->field_0x0 = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->field_0x4 = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

// ---- setSeed_71a04.cpp ----
void setSeed_71a04(AbyssEngine::AERandom *self, long long seed)
{
    self->field_0x0 = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->field_0x4 = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
}

// ---- reset_71a30.cpp ----
void reset_71a30(AbyssEngine::AERandom *self)
{
    int seed = time(0);
    self->field_0x0 = 0xdeece66dU ^ static_cast<uint32_t>(seed);
    self->field_0x4 = (static_cast<uint32_t>(seed >> 31) & 0xffffU) ^ 5U;
}

// ---- next_71a54.cpp ----
uint32_t next_71a54(AbyssEngine::AERandom *self, int bits)
{
    uint32_t lo = self->field_0x0;
    uint32_t hi = self->field_0x4;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);
    uint32_t high = newHi & 0xffffU;

    self->field_0x0 = newLo;
    self->field_0x4 = high;

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
    uint32_t lo = self->field_0x0;
    uint32_t hi = self->field_0x4;
    uint64_t seed = (static_cast<uint64_t>(hi) << 32) | lo;
    seed = seed * 0x5deece66dULL + 0xbULL;
    uint32_t newLo = static_cast<uint32_t>(seed);
    uint32_t newHi = static_cast<uint32_t>(seed >> 32);

    self->field_0x0 = newLo;
    self->field_0x4 = newHi & 0xffffU;
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
