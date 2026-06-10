#ifndef GOF2_AERANDOM_H
#define GOF2_AERANDOM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
struct AERandom;
}

static inline uint32_t &ae_u32(AbyssEngine::AERandom *self, uint32_t offset)
{
    return *reinterpret_cast<uint32_t *>(reinterpret_cast<char *>(self) + offset);
}

static inline const uint32_t &ae_u32(const AbyssEngine::AERandom *self, uint32_t offset)
{
    return *reinterpret_cast<const uint32_t *>(reinterpret_cast<const char *>(self) + offset);
}

struct AERandom { void* _opaque; };  // no offset accesses observed
#endif
