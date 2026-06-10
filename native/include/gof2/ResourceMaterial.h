#ifndef GOF2_RESOURCEMATERIAL_H
#define GOF2_RESOURCEMATERIAL_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct ResourceMaterial {
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
#endif
