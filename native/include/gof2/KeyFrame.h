#ifndef GOF2_KEYFRAME_H
#define GOF2_KEYFRAME_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct KeyFrame {
    // @portable-fields
    unsigned char _pad_0[72];
    int f_48; // 0x48
    unsigned char _pad_4c[4];
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c

    KeyFrame();
};
#endif
