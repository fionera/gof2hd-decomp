#include "engine/render/ResourceMaterial.h"

namespace AbyssEngine {

ResourceMaterial::ResourceMaterial(uint16_t texId, uint16_t texId2, BlendMode blend)
{
    blendMode = (int)blend;
    field_14  = 0;
    field_18  = 0;
    field_1c  = 0x3f800000; // 1.0f bit pattern
    field_20  = 0;
    field_24  = 0;
    for (int i = 0; i != 8; ++i)
        texIndices[i] = 0xffff;
    texIndices[1] = texId2;
    texIndices[0] = texId;
}

ResourceMaterial::ResourceMaterial(uint16_t texId, BlendMode blend)
{
    blendMode = (int)blend;
    field_14  = 0;
    field_18  = 0;
    field_1c  = 0x3f800000; // 1.0f bit pattern
    field_20  = 0;
    field_24  = 0;
    for (int i = 0; i != 8; ++i)
        texIndices[i] = 0xffff;
    texIndices[0] = texId;
}

} // namespace AbyssEngine
