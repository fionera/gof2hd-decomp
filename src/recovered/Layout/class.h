#ifndef WORK_CLASSES_LAYOUT_SRC_CLASS_H
#define WORK_CLASSES_LAYOUT_SRC_CLASS_H

// Galaxy on Fire 2 — Layout class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace; only some field/arg types live in
// AbyssEngine). Field offsets recovered from per-method target disassembly. We do
// NOT model a full layout — methods access fields via byte-offset casts from `this`.
//
// Known field offsets (partial; large object ~0x414 bytes):
//   0x3b0 int   drawColor          0x3bc int  helpButtonId/width arg
//   0x3c0 uint8 helpPressed flag   0x3c4 ptr  help window/button
//   0x400 uint8 fading flag        0x401 uint8 fadeOut flag
//   0x404 uint  fadeColor          0x408 int  fadeProgress   0x40c int fadeDuration
//   0x410 uint8 fillScreen flag

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Layout owns three TouchButton* (0x3b4/0x3b8/0x3bc) and a ChoiceWindow* (0x3c4),
// each deleted in the destructor. Declared so it demangles to Layout::~Layout();
// all other methods are authored as extern "C" wrappers.
struct Layout {
    ~Layout();
};

// AbyssEngine::String — 12-byte object passed by value as a trivially-copied
// aggregate; heap members modeled opaquely.
struct String12 { uint32_t a, b, c; };

namespace AbyssEngine {
// AbyssEngine::String — 12-byte object. Modeled with the ctors/dtor we need so the
// compiler emits the engine's String(const String&, bool) copy-ctor and ~String()
// at the right points; bodies live in the engine (linked elsewhere).
struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    String(const String &other, bool copy);
    ~String();
    String &operator=(const String &other);
};
String operator+(const String &left, const String &right);
} // namespace AbyssEngine
using AbyssEngine::String;

// placement new
inline void *operator new(uint32_t, void *p) noexcept { return p; }

// Stack-protector guard (the engine resolves this to a PC-relative game global).
extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

template <class T>
struct Array {
    uint32_t length;   // +0x00
    T *data;           // +0x04
    uint32_t cap;      // +0x08
};

// Field accessor via byte offset.
template <class T>
static inline T &F(Layout *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
