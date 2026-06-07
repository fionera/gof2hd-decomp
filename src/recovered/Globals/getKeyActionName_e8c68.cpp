#include "class.h"

// Globals::getKeyActionName(int) returns a String by value. The default ctor returns void,
// so the compiler keeps a frame and restores the sret pointer (no tail call). The int arg
// is unused.
extern "C" void AEString_ctor_default(void *out);

struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Globals_getKeyActionName(int action)
{
    (void)action;
    RetStr r;
    AEString_ctor_default(&r);
    return r;
}
