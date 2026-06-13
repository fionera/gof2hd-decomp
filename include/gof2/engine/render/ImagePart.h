#ifndef GOF2_IMAGEPART_H
#define GOF2_IMAGEPART_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// ImagePart — top-level class (only arg types are namespaced).
// Layout from ctor/draw decompiles:
//   +0x00  uint   id            (image2d handle)
//   +0x04  int    field_04      (flags; draw ORs with 1)
//   +0x08  int    pos_y
//   +0x0c  int    scale_x       (image width)
//   +0x10  int    scale_y       (image height)

void *operator new(__SIZE_TYPE__ size);

class ImagePart {
public:
    // @portable-fields
    int id; // 0x0
    int f_4; // 0x4
    int pos_y; // 0x8
    int scale_x; // 0xc
    int scale_y; // 0x10

    // ---- methods (converted from free functions) ----
    ImagePart * ctor(unsigned id, int field04, int posY);
    void draw(int x, int y, bool b);
};
#endif
