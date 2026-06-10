#pragma once

#include <stdint.h>

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

