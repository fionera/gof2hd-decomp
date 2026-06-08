#include "class.h"

// AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, BlendMode)
namespace AbyssEngine {

ResourceMaterial::ResourceMaterial(unsigned short texId, BlendMode blend)
{
    i32(this, 0x10) = (int)blend;
    i32(this, 0x14) = 0;
    i32(this, 0x18) = 0;
    i32(this, 0x1c) = 0x3f800000; // 1.0f bit pattern from DAT
    i32(this, 0x20) = 0;
    i32(this, 0x24) = 0;
    for (int i = 0; i != 8; ++i) {
        u16(this, (uint32_t)(i * 2)) = 0xffff;
    }
    u16(this, 0x0) = texId;
}

} // namespace AbyssEngine
