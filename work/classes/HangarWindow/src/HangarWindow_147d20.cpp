#include "class.h"

// Two PC-rel globals: a window-open counter (set to 1) and a layout/config singleton
// whose fields 0x238..0x250 are copied into this 0x100..0x118.
__attribute__((visibility("hidden"))) extern int *g_hw_openCounter;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

// NEAR: NEON block copies match (vld1/vst1.32), but clang schedules the many independent
// field-init stores in a different order and picks r12 (no frame) where the target uses r4
// (push). Structurally correct; instruction ordering differs.
struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };

extern "C" void HangarWindow_ctor(HangarWindow *self)
{
    int *cnt = g_hw_openCounter;
    void *lay = *g_hw_layout;

    F<void *>(self, 0x80) = 0;
    F<void *>(self, 0x84) = 0;
    F<uint8_t>(self, 0x3c) = 0;
    F<uint8_t>(self, 0xac) = 0;
    // zero 4 words at 0x14..0x20 (a zeroed Vector slot) via a 16-byte NEON store
    Blk16 zero = {0, 0, 0, 0};
    *(Blk16 *)((char *)self + 0x14) = zero;
    F<uint8_t>(self, 0x11e) = 0;
    *cnt = 1;
    F<uint8_t>(self, 0x130) = 0;
    F<uint8_t>(self, 0xd0) = 0;
    F<void *>(self, 0x4) = 0;
    F<void *>(self, 0x24) = 0;
    F<void *>(self, 0x90) = 0;
    F<uint8_t>(self, 0xae) = 0;

    // copy lay[0x238..0x244] (16 bytes) into this[0x100..0x10c]
    *(Blk16 *)((char *)self + 0x100) = *(Blk16 *)((char *)lay + 0x238);
    F<int>(self, 0x110) = G<int>(lay, 0x248);
    F<int>(self, 0x114) = G<int>(lay, 0x24c);
    F<int>(self, 0x118) = G<int>(lay, 0x250);
}
