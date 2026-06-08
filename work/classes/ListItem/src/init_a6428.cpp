#include "class.h"

// ListItem::init() — zero/sentinel-init all fields. Returns this+4.
// The this+4 16-byte block (fields 4,8,0xc,0x10) is the NEON store (vst1.32);
// the -1 sentinels (0x28..0x34, 0x3c..0x40) and the 0x14..0x20 zeros are strd
// pairs. NOTE: clang's SLP vectorizer additionally merges the 0x14..0x20 pair into
// a single NEON store here, which the target keeps as two strd — a residual
// 1-instruction-class difference we could not coax away at -Oz.
extern "C" void *ListItem_init(ListItem *self) {
    self->f_14 = 0;
    self->f_1c = 0;
    self->f_24 = 0;
    self->f_28 = -1;
    self->f_2c = -1;
    self->f_30 = -1;
    self->f_34 = -1;
    self->f_38 = 0;
    self->f_3c = -1;
    self->f_40 = -1;
    self->f_44 = 0;
    unsigned *p4 = (unsigned *)((char *)self + 4);
    p4[0] = 0; p4[1] = 0; p4[2] = 0; p4[3] = 0;
    return (char *)self + 4;
}
