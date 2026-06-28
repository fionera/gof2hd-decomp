#ifndef GOF2_IMAGE2D_H
#define GOF2_IMAGE2D_H
#include <cstdint>

namespace AbyssEngine {
    class Mesh;

    struct Image2D {
        Mesh *mesh;
        uint32_t field_0x4;
        uint16_t atlasW;
        uint16_t atlasH;
        uint16_t offX;
        uint16_t offY;
        uint16_t sizeX;
        uint16_t sizeY;
    };
}

#endif
