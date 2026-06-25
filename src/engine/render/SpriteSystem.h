#ifndef GOF2_SPRITESYSTEM_H
#define GOF2_SPRITESYSTEM_H
#include <cstdint>

namespace AbyssEngine {
    class Mesh;



    struct SpriteSystem {
        uint16_t count;
        uint16_t pad0x2;
        float *posCpu;
        int16_t *sizeCpu;
        uint8_t sharedSize;
        uint8_t pad0xd[3];
        Mesh *mesh;
    };
}

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(sizeof(AbyssEngine::SpriteSystem) == 0x14, "SpriteSystem layout");
static_assert(offsetof(AbyssEngine::SpriteSystem, posCpu) == 0x4, "SpriteSystem.posCpu");
static_assert(offsetof(AbyssEngine::SpriteSystem, sizeCpu) == 0x8, "SpriteSystem.sizeCpu");
static_assert(offsetof(AbyssEngine::SpriteSystem, sharedSize) == 0xc, "SpriteSystem.sharedSize");
static_assert(offsetof(AbyssEngine::SpriteSystem, mesh) == 0x10, "SpriteSystem.mesh");
#endif

#endif
