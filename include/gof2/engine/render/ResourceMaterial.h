#ifndef GOF2_RESOURCEMATERIAL_H
#define GOF2_RESOURCEMATERIAL_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Galaxy on Fire 2 -- AbyssEngine::ResourceMaterial (Android libgof2hdaa.so, armv7 Thumb).
// Two constructor overloads. Fields accessed via byte-offset casts from `this`.


namespace AbyssEngine {

enum BlendMode { BlendMode_dummy };

class ResourceMaterial {
public:
    // @portable-fields
    uint16_t f_0; // 0x0
    uint16_t f_2; // 0x2
    unsigned char _pad_4[12];
    int f_10; // 0x10
    int f_14; // 0x14
    int f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24

    ResourceMaterial(unsigned short texId, unsigned short texId2, BlendMode blend);
    ResourceMaterial(unsigned short texId, BlendMode blend);
};

} // namespace AbyssEngine

// Field accessors via byte offset.
#endif
