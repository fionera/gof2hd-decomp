#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) -> void (0x6f028)
extern "C" void String_copy_ctor(void *out, void *src, bool);

// SolarSystem::getName() -> String by value (sret in r0, this in r1).
// Copies the String member at +0xc. The copy-ctor returns void, so the compiler
// keeps a frame and restores the sret pointer.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr SolarSystem_getName(SolarSystem *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0xc, false);
    return r;
}
