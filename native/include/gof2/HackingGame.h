#ifndef GOF2_HACKINGGAME_H
#define GOF2_HACKINGGAME_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
static inline int32_t &I(HackingGame *self, uint32_t off)
{
    return F<int32_t>(self, off);
}

static inline uint8_t &B(HackingGame *self, uint32_t off)
{
    return F<uint8_t>(self, off);
}

struct HackingGame { void* _opaque; };  // no offset accesses observed
#endif
