#ifndef GOF2_MATERIAL_H
#define GOF2_MATERIAL_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct Material {
    // @portable-fields
    int f_0; // 0x0
    unsigned char _pad_4[28];
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    unsigned char _pad_2c[60];
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70

    Material();
    Material(Material *other);
    ~Material();
};
#endif
