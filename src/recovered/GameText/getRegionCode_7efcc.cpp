#include "class.h"

extern "C" void AEString_ctor_cstr(void *self, const char *text, bool copy);
// PC-relative region-code C string.
extern const char gRegionCodeStr[] __attribute__((visibility("hidden")));

struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

// GameText::getRegionCode() -> returns an AbyssEngine::String by value (sret).
extern "C" RetStr GameText_getRegionCode()
{
    RetStr r;
    AEString_ctor_cstr(&r, gRegionCodeStr, false);
    return r;
}
