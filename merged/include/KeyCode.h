#ifndef WORK_CLASSES_KEYCODE_SRC_CLASS_H
#define WORK_CLASSES_KEYCODE_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::KeyCode (Android libgof2hdaa.so, armv7 Thumb).
// Tiny value type: an int code at +0x00 followed by an embedded String at +0x04.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

// String -- 12-byte value type (text*, field_04, size). >8 bytes => passed by ref/sret.
struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;
    String &operator=(const String &other) noexcept;
};

struct KeyCode {
    int32_t code;     // +0x00
    String name;      // +0x04 (12 bytes)
    KeyCode &operator=(const KeyCode &other);
};

} // namespace AbyssEngine

#endif
