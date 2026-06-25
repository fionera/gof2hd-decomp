#ifndef GOF2_AELOADEDTEXTURE_H
#define GOF2_AELOADEDTEXTURE_H
#include <cstdint>

namespace AbyssEngine {


    struct AELoadedTexture {
        uint32_t glId;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t field_0xc;
        uint32_t field_0x10;
        uint8_t isCube;
        uint8_t valid;
        uint16_t pad0x16;
        uint32_t byteSize;
    };
}

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(offsetof(AbyssEngine::AELoadedTexture, isCube) == 0x14, "AELoadedTexture.isCube");
static_assert(offsetof(AbyssEngine::AELoadedTexture, valid) == 0x15, "AELoadedTexture.valid");
static_assert(offsetof(AbyssEngine::AELoadedTexture, byteSize) == 0x18, "AELoadedTexture.byteSize");
#endif

#endif
