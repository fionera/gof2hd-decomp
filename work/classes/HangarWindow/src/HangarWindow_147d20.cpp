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

    self->f_80 = 0;
    self->f_84 = 0;
    self->f_3c = 0;
    self->f_ac = 0;
    // zero 4 words at 0x14..0x20 (a zeroed Vector slot) via a 16-byte NEON store
    Blk16 zero = {0, 0, 0, 0};
    *(Blk16 *)((char *)self + 0x14) = zero;
    F<uint8_t>(self, 0x11e) = 0;
    *cnt = 1;
    self->f_130 = 0;
    self->f_d0 = 0;
    self->f_4 = 0;
    self->f_24 = 0;
    self->f_90 = 0;
    F<uint8_t>(self, 0xae) = 0;

    // copy lay[0x238..0x244] (16 bytes) into this[0x100..0x10c]
    *(Blk16 *)((char *)self + 0x100) = *(Blk16 *)((char *)lay + 0x238);
    self->f_110 = G<int>(lay, 0x248);
    self->f_114 = G<int>(lay, 0x24c);
    self->f_118 = G<int>(lay, 0x250);
}
