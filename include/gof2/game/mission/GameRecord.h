#ifndef GOF2_GAMERECORD_H
#define GOF2_GAMERECORD_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - GameRecord (top-level class, no AbyssEngine:: namespace).
// Embeds two AbyssEngine::String members at +0x188 and +0x194, plus a heap buffer
// at +0x00 and many scalar fields. Fields reached via byte-offset casts from `this`.

// Field accessor via byte offset.

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);
void __aeabi_memclr8(void *dst, uint32_t n);
void __aeabi_memclr4(void *dst, uint32_t n);
void __aeabi_memclr(void *dst, uint32_t n);
}

class GameRecord {
public:
    void* _opaque;          // +0x00 heap buffer / raw byte-offset access throughout (coverage-mode translation)
    char  _pad_04[0x184];   // +0x04 scalar fields reached via byte-offset casts from `this`
    String nameString;      // +0x188 embedded AbyssEngine::String
    String descString;      // +0x194 embedded AbyssEngine::String
    GameRecord();
    ~GameRecord();

    // ---- methods (converted from free functions) ----
    void load();
};
#endif
