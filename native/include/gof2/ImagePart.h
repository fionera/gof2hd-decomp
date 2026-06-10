#ifndef GOF2_IMAGEPART_H
#define GOF2_IMAGEPART_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct ImagePart {
    // @portable-fields
    int id; // 0x0
    int f_4; // 0x4
    int pos_y; // 0x8
    int scale_x; // 0xc
    int scale_y; // 0x10
};
#endif
