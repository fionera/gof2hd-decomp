#ifndef GOF2_AERANDOM_H
#define GOF2_AERANDOM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
// Java-style 48-bit linear congruential PRNG (java.util.Random equivalent).
class AERandom {
public:
    uint32_t seedLow;     // +0x0  PRNG state low 32 bits
    uint32_t seedHigh;    // +0x4  PRNG state high 16 bits

    AERandom();                 // time-seeded
    explicit AERandom(long long seed);
    ~AERandom();

    void setSeed(long long seed);
    void reset();               // re-seed from current time

    uint32_t next(int bits);    // raw next(bits) generator
    int nextInt();              // 31-bit non-negative int
    int nextInt(int bound);     // [0, bound)
};
} // namespace AbyssEngine
#endif
