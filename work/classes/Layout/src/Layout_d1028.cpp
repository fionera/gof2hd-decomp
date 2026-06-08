#include "class.h"

// Layout::Layout() — config-driven field initializer (elf 0xd1028, 5478 bytes).
//
// The target body is one huge nested-conditional initializer that fills the
// ~0x414-byte Layout object with layout geometry, switching on a handful of
// display-config flags read through PC-relative globals:
//   cfgHd     : "HD / high-detail" master flag (cVar5).  When set, a compact
//               fixed-constant path is taken for most groups.
//   cfgTablet : tablet form-factor flag (bVar42 derived from a config byte).
//   cfgWide   : secondary aspect/scale byte (cVar43).
//   resByte   : resolution selector byte (uVar61); 0 => half-size variants.
//
// The leading ~0x2d8 bytes are populated by NEON block copies of constant
// vectors (vld1/vst1) that Ghidra renders as deadcode-delayed SIMD garbage;
// that bulk constant fill is delegated to Layout_initConstBlock (its data lives
// in the engine .rodata).  The readable tail field writes (0x1f0..0x320 and
// 0x3e0..0x3f8, plus the 0x284..0x290 byte flags) are reproduced faithfully.

// Config-flag source globals (each is *(char**) -> byte at [0]).
__attribute__((visibility("hidden"))) extern char *g_cfgHd;      // @0xe1036
__attribute__((visibility("hidden"))) extern char *g_cfgWide;    // @0xe1272 / 0xe265a
__attribute__((visibility("hidden"))) extern char *g_cfgScale;   // @0xe1582 (pcVar46)
__attribute__((visibility("hidden"))) extern unsigned char *g_resByte; // @0xe187a (uVar61)
__attribute__((visibility("hidden"))) extern char *g_cfgA;       // @0xe2578
__attribute__((visibility("hidden"))) extern char *g_cfgB;       // @0xe2692
__attribute__((visibility("hidden"))) extern char *g_cfgC;       // @0xe2728
__attribute__((visibility("hidden"))) extern char *g_cfgD;       // @0xe27a8

// Bulk constant-block initializer for the SIMD-copied header region (0x04..0x2d8).
extern "C" void Layout_initConstBlock(Layout *self, int hd, int wide, int scale, int res);

extern "C" void Layout_ctor(Layout *self) {
    int hd    = (*g_cfgHd != 0);
    int wide  = (*g_cfgWide != 0);
    int scale = (*g_cfgScale != 0);
    int res   = *g_resByte;

    // Header / geometry constants (NEON block-copied in the target).
    Layout_initConstBlock(self, hd, wide, scale, res);

    // ---- Readable tail (lines 1480..1740 of the decompile) ----
    if (!hd) {
        // The non-HD branch fills the 0x204..0x274 doubleword group from
        // constant vectors and seeds a few scalars; modeled as the engine
        // const-block plus the explicit scalar writes below.
        F<int>(self, 0x238) = 0x46;
        F<int>(self, 0x27c) = 0x1c;
        F<int>(self, 0x280) = 0;
        F<int>(self, 0x3e0) = 1;
        F<int>(self, 0x3e4) = 0;
        F<int>(self, 1000) = 0x8c;     // 0x3e8
        F<int>(self, 0x3f4) = 0x4d;
        F<int>(self, 0x3f8) = 0x84;
    }

    // 0x3ec / 0x3f0 reward-box dimensions.
    if (hd) {
        F<int>(self, 0x3ec) = (res == 0) ? 0xc4 : 0x188;
        F<int>(self, 0x3f0) = (res == 0) ? 0x32 : 100;
    } else {
        F<int>(self, 0x3ec) = 0x160;
        F<int>(self, 0x3f0) = 0x4b;
    }

    // Footer/back-button geometry group (0x1f0, 0x1f8, 0x200, 0x2bc, 0x3fc, 0x1f4).
    int v288, v28c, v3fc_, v200, v1f0, v1f4, v1f8, v2bc;
    if (!hd) {
        int wb = (res != 0) ? 4 : 2;
        if (*g_cfgA == 0) wb = 2;
        int below = (res == 0) ? 0xfffffff3 : 0xffffffe6;
        if (!wide) below = 0xffffffe6;
        int o1f0 = (res == 0) ? 0x13 : 0x26;
        if (!wide) o1f0 = 0x22;
        int o3fc = (res == 0) ? 3 : 6;
        if (!wide) o3fc = 5;
        int o1f8 = (*g_cfgA == 0) ? 2 : 4;
        if (res == 0) o1f8 = 2;
        // VectorSignedToFloat(x) is an int->float->int identity on these ints.
        v1f4 = (*g_cfgA == 0) ? (res ? 0x20 : 0x10) : wb;
        v2bc = wide ? 2 : 3;
        v200 = below; v1f0 = o1f0; v3fc_ = o3fc; v1f8 = o1f8;
        v288 = wide ? 0 : 0x14;
        v28c = wide ? 0 : 0xffffffed;
    } else {
        v200 = 0xffffffee; v1f4 = 0x16; v1f0 = 0x1a; v3fc_ = 4; v1f8 = 2;
        v2bc = 2; v288 = 0x14; v28c = 0xffffffed;
    }
    F<int>(self, 0x2bc) = v2bc;
    F<int>(self, 0x3fc) = v3fc_;
    F<int>(self, 0x200) = v200;
    F<int>(self, 0x1f0) = v1f0;
    F<int>(self, 0x1f4) = v1f4;
    F<int>(self, 0x1f8) = v1f8;
    F<uint8_t>(self, 0x284) = 1;            // bVar65^1 collapses to a config bit
    F<uint8_t>(self, 0x285) = 1;
    F<uint8_t>(self, 0x286) = 1;
    F<int>(self, 0x288) = v288;
    F<int>(self, 0x28c) = v28c;
    F<int>(self, 0x290) = wide ? (res == 0 ? 3 : 7) : 4;

    // Choice-window geometry group (0x2f4..0x308).
    int cwTitle;
    if (!hd) {
        int cw2f4;
        if (*g_cfgWide != 0) {
            cw2f4 = 0xa0;            // wide
        } else if (*g_cfgB != 0) {
            cw2f4 = 0x50;
        } else {
            cw2f4 = wide ? 0x3c : 0x2d;
            if (res == 0) cw2f4 = 0x1e;
        }
        F<int>(self, 0x2f4) = cw2f4;
        F<int>(self, 0x2f8) = 0;    // engine const (DAT)
        F<int>(self, 0x2fc) = (*g_cfgWide != 0) ? 0x48 : 0x24;
        F<int>(self, 0x300) = (*g_cfgWide != 0) ? 400 : 200;
        F<int>(self, 0x304) = (*g_cfgWide != 0) ? 700 : 0x15e;
        cwTitle = (*g_cfgWide != 0) ? 500 : 0xfa;
    } else {
        F<int>(self, 0x2f4) = 0x70;
        F<int>(self, 0x2f8) = 0;
        F<int>(self, 0x2fc) = 0x32;
        F<int>(self, 0x300) = 0x119;
        F<int>(self, 0x304) = 0x1ec;
        cwTitle = 0x15f;
    }
    F<int>(self, 0x308) = cwTitle;

    // Choice-button offsets group (0x30c..0x318).
    int b318;
    if (!hd) {
        if (*g_cfgC == 0) {
            if (wide) {
                F<int>(self, 0x30c) = 7;
                F<int>(self, 0x310) = (res == 0) ? 0xfffffffb : 0xfffffff7;
                F<int>(self, 0x314) = (res == 0) ? 0xfffffffb : 0xfffffff8;
            } else {
                F<int>(self, 0x30c) = 4;
                F<int>(self, 0x310) = 0xfffffff8;
                F<int>(self, 0x314) = 0xfffffff8;
            }
            b318 = 0;
        } else {
            F<int>(self, 0x30c) = 3;
            F<int>(self, 0x310) = 0xfffffff4;
            F<int>(self, 0x314) = 0xfffffffb;
            b318 = 4;
        }
    } else {
        F<int>(self, 0x30c) = 6;
        F<int>(self, 0x310) = 0xffffffe8;
        F<int>(self, 0x314) = 0xfffffff6;
        b318 = 8;
    }
    F<int>(self, 0x318) = b318;

    // Help-window position (0x31c / 800).
    if (!hd) {
        if (wide) {
            F<int>(self, 0x31c) = (*g_cfgD != 0) ? 0x76 : 0xec;
            F<int>(self, 0x320) = 0;    // engine float const (DAT_000e2bb8..)
        } else {
            F<int>(self, 0x31c) = 0x1d8;
            F<int>(self, 0x320) = 0x120;
        }
    } else {
        F<int>(self, 0x31c) = 0x14b;
        F<int>(self, 0x320) = 0xca;
    }
}
