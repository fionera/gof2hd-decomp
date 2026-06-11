#ifndef GOF2_AERANDOM_H
#define GOF2_AERANDOM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
struct AERandom {
    uint32_t seedLow;    // +0x0  PRNG state low
    uint32_t seedHigh;    // +0x4  PRNG state high
};
} // namespace AbyssEngine
#endif
