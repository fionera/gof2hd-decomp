#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) -> void
extern "C" void String_copy_ctor(void *out, void *src, bool);

// Returns the name String (stored at offset 0x0) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Wanted_getName(Wanted *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x00, false);
    return r;
}
