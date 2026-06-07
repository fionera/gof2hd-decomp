#include "class.h"

// ListItem::init() — zero/sentinel-init all fields. Returns this+4.
// The this+4 16-byte block (fields 4,8,0xc,0x10) is the NEON store (vst1.32);
// the -1 sentinels (0x28..0x34, 0x3c..0x40) and the 0x14..0x20 zeros are strd
// pairs. NOTE: clang's SLP vectorizer additionally merges the 0x14..0x20 pair into
// a single NEON store here, which the target keeps as two strd — a residual
// 1-instruction-class difference we could not coax away at -Oz.
extern "C" void *ListItem_init(ListItem *self) {
    F<long long>(self, 0x14) = 0;
    F<long long>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = -1;
    F<int>(self, 0x2c) = -1;
    F<int>(self, 0x30) = -1;
    F<int>(self, 0x34) = -1;
    F<uint8_t>(self, 0x38) = 0;
    F<int>(self, 0x3c) = -1;
    F<int>(self, 0x40) = -1;
    F<uint16_t>(self, 0x44) = 0;
    unsigned *p4 = (unsigned *)((char *)self + 4);
    p4[0] = 0; p4[1] = 0; p4[2] = 0; p4[3] = 0;
    return (char *)self + 4;
}
