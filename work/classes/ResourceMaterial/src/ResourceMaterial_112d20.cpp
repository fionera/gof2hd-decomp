#include "class.h"

// AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, BlendMode)
namespace AbyssEngine {

ResourceMaterial::ResourceMaterial(unsigned short texId, BlendMode blend)
{
    this->f_10 = (int)blend;
    this->f_14 = 0;
    this->f_18 = 0;
    this->f_1c = 0x3f800000; // 1.0f bit pattern from DAT
    this->f_20 = 0;
    this->f_24 = 0;
    for (int i = 0; i != 8; ++i) {
        u16(this, (uint32_t)(i * 2)) = 0xffff;
    }
    this->f_0 = texId;
}

} // namespace AbyssEngine
