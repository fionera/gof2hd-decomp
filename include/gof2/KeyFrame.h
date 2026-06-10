#ifndef GOF2_KEYFRAME_H
#define GOF2_KEYFRAME_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Galaxy on Fire 2 -- AbyssEngine::KeyFrame (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name: AbyssEngine::KeyFrame::KeyFrame().
// Field offsets recovered from the target disassembly; access via byte-offset casts.
//
// Object layout (recovered from ctor):
//   +0x00  block cleared to 0 over the first 0x48 bytes
//   +0x0c  Vector  scale-ish vector A   (default 1,1,1)
//   +0x30  Vector  scale-ish vector B   (default 1,1,1)
//   +0x48  float   weight               (default 1.0)
//   +0x50  int     0
//   +0x54  int     0
//   +0x58  int     0
//   +0x5c  int     0


extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memclr4(void *dst, uint32_t n);

namespace AbyssEngine {
namespace AEMath {



} // namespace AEMath

struct KeyFrame {
    // @portable-fields
    unsigned char _pad_0[12];           // 0x00..0x0c
    AEMath::Vector field_0xc;           // 0x0c  scale-ish vector A (default 1,1,1)
    unsigned char _pad_18[24];          // 0x18..0x30
    AEMath::Vector field_0x30;          // 0x30  scale-ish vector B (default 1,1,1)
    unsigned char _pad_3c[12];          // 0x3c..0x48
    int f_48; // 0x48
    unsigned char _pad_4c[4];
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c

    KeyFrame();
};

} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------
#endif
