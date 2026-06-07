#include "class.h"

// Globals::replaceKeyBindingTokens(String const&) returns a String by value (sret in r0).
// Body copy-constructs the return slot from the source string (in r2), flag=false.
// The copy-ctor returns void -> frame kept, sret pointer restored (no tail call).
extern "C" void AEString_ctor_copy(void *dst, void *src, bool flag);

struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Globals_replaceKeyBindingTokens(void *unused, void *src)
{
    (void)unused;
    RetStr r;
    AEString_ctor_copy(&r, src, false);
    return r;
}
