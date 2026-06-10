#ifndef GOF2_KEYCODE_H
#define GOF2_KEYCODE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::KeyCode (Android libgof2hdaa.so, armv7 Thumb).
// Tiny value type: an int code at +0x00 followed by an embedded String at +0x04.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.


namespace AbyssEngine {

// String -- 12-byte value type (text*, field_04, size). >8 bytes => passed by ref/sret.




} // namespace AbyssEngine

struct KeyCode { void* _opaque; };  // no offset accesses observed
#endif
