#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) -> 0x6f028
extern "C" void String_copy_ctor(void *out, void *src, bool);
struct RetStr { uint32_t a, b, c; };

// Returns the client name String (offset 0x10) by value. The void copy-ctor forces
// a frame + non-tail blx (the sret r0 must be restored).
extern "C" RetStr Mission_getClientName(Mission *self) {
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x10, false);
    return r;
}
