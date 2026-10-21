#ifndef GOF2_RESOURCEMATERIAL_H
#define GOF2_RESOURCEMATERIAL_H

#include <cstdint>
#include "engine/render/RenderEnums.h"
#include "engine/render/MaterialShading.h"

namespace AbyssEngine {
    // Material resource descriptor (40 bytes): up to 8 texture resource ids (0xffff = unused),
    // the blend mode and the shading parameters PaintCanvas copies into the runtime Material
    // (flags1/flags2/vec, see PaintCanvas's material creation).
    class ResourceMaterial {
    public:
        // Defined in-class: weak (inline) C2 symbols in the original, emitted by GameResource.
        ResourceMaterial(uint16_t texId, uint16_t texId2, BlendMode blend) : blendMode(static_cast<int>(blend)) {
            for (int i = 0; i < 8; ++i)
                this->texIndices[i] = 0xffff;
            this->texIndices[0] = texId;
            this->texIndices[1] = texId2;
        }

        ResourceMaterial(uint16_t texId, BlendMode blend) : blendMode(static_cast<int>(blend)) {
            for (int i = 0; i < 8; ++i)
                this->texIndices[i] = 0xffff;
            this->texIndices[0] = texId;
        }

        uint16_t texIndices[8];
        int blendMode;
        MaterialShading shading;
    };
}

#endif
