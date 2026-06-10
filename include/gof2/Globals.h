#ifndef GOF2_GLOBALS_H
#define GOF2_GLOBALS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - Globals class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Most members are static-style helpers.
// Field offsets recovered per-method from target disasm; access via byte-offset casts.


struct Globals;

// AbyssEngine::String - 12-byte value type (text*, size, ...). Pass-by-value uses the
// >8-byte struct ABI (sret / on stack), matching the target.
namespace AbyssEngine {

}
typedef AbyssEngine::String String;



// Field accessor via byte offset (used by the few stateful methods).

struct Globals {
    Array<int>* field_0x4;              // +0x4  (sound-resource id list)
    int field_0x34;                     // +0x34
    void* field_0x3c;                   // +0x3c
    void* field_0x40;                   // +0x40
    void* field_0x48;                   // +0x48
    unsigned int* field_0x54;           // +0x54
    int field_0xac;                     // +0xac
    unsigned short field_0x110;         // +0x110
    int field_0x114;                    // +0x114
    int field_0x14c;                    // +0x14c
};
#endif
