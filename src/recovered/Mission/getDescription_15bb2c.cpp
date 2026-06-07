#include "class.h"

// AbyssEngine::String::String(String* out, const char* cstr, bool) -> 0x6ee18
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
struct RetStr { uint32_t a, b, c; };

// Returns a fixed description String built from a string literal.
extern "C" RetStr Mission_getDescription(Mission *self) {
    RetStr r;
    String_cstr_ctor(&r, "", false);
    return r;
}
