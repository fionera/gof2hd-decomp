#ifndef GOF2_STRING_H
#define GOF2_STRING_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - AbyssEngine::String. Android libgof2hdaa.so, armv7 Thumb.
// Namespaced class (namespace AbyssEngine). Recovered layout:
//   +0x00  vtable pointer
//   +0x04  ushort* data   (UTF-16 / wide character buffer, NUL-terminated)
//   +0x08  uint   length  (number of code units, excluding terminator)
// Fields are accessed via byte-offset casts; we do NOT model a full layout.


namespace AbyssEngine {
struct String;
}
typedef AbyssEngine::String String;

// Array<T> container header (length / data / capacity) used by Split / SplitTags.


// Field accessors via byte offset.

struct String {
    void* field_0x0;                    // +0x0
    void* field_0x4;                    // +0x4
    int field_0x8;                      // +0x8
};
#endif
