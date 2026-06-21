#include "engine/render/ResourceMaterial.h"

namespace AbyssEngine {

// field_1c is initialised to 0x3f800000, the IEEE-754 bit pattern of 1.0f.

ResourceMaterial::ResourceMaterial(uint16_t texId, uint16_t texId2, BlendMode blend)
{
    this->blendMode = static_cast<int>(blend);
    this->field_14  = 0;
    this->field_18  = 0;
    this->field_1c  = 0x3f800000;
    this->field_20  = 0;
    this->field_24  = 0;
    for (int i = 0; i != 8; ++i)
        this->texIndices[i] = 0xffff;
    this->texIndices[1] = texId2;
    this->texIndices[0] = texId;
}

ResourceMaterial::ResourceMaterial(uint16_t texId, BlendMode blend)
{
    this->blendMode = static_cast<int>(blend);
    this->field_14  = 0;
    this->field_18  = 0;
    this->field_1c  = 0x3f800000;
    this->field_20  = 0;
    this->field_24  = 0;
    for (int i = 0; i != 8; ++i)
        this->texIndices[i] = 0xffff;
    this->texIndices[0] = texId;
}

} // namespace AbyssEngine
