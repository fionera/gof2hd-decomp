#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) -> void
extern "C" void String_copy_ctor(void *out, void *src, bool);

// Returns String by value. The copy-ctor returns void, so the compiler cannot
// assume r0 survives the call and must keep a frame + restore the sret pointer.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getName(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x00, false);
    return r;
}
