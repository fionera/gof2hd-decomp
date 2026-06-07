#include "class.h"

// AbyssEngine::String::String(String* out, const char* cstr, bool) -> void (0x6ee18)
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);

// FileInterfaceAndroid::GetDirPreFix() -> String built from a literal prefix (sret in r0).
// The copy ctor returns void, so the compiler keeps a frame and restores the sret pointer.
extern const char kDirPreFix[] __attribute__((visibility("hidden")));

struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr FileInterfaceAndroid_GetDirPreFix()
{
    RetStr r;
    String_ctor_cstr(&r, kDirPreFix, false);
    return r;
}
