#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) -> void.
// Returns String by value (sret r0, this r1). Copy-ctor returns void, so the
// compiler keeps a frame + restores the sret pointer.
extern "C" void String_copy_ctor(void *out, void *src, bool);
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr PlayerFixedObject_getName(PlayerFixedObject *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x1ac, false);
    return r;
}
