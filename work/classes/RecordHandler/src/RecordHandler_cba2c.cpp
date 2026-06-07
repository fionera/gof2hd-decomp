#include "class.h"

// String literal pointers (each via a hidden pc-rel reloc).
__attribute__((visibility("hidden"))) extern const char RH_lit0[];
__attribute__((visibility("hidden"))) extern const char RH_lit1[];
__attribute__((visibility("hidden"))) extern const char RH_lit2[];

extern "C" void String_default_ctor(void *s);
extern "C" void String_cstr_ctor(void *s, const char *cs, bool b);
extern "C" void String_assign(void *dst, void *src);
extern "C" void String_dtor(void *s);

// RecordHandler::RecordHandler() — default-constructs the three String members at
// +0x8, +0x14, +0x20, then assigns each from a literal, and returns `this`.
extern "C" RecordHandler *RecordHandler_ctor(RecordHandler *self)
{
    char tmp[12];
    char *m0 = (char *)self + 0x8;
    char *m1 = (char *)self + 0x14;
    char *m2 = (char *)self + 0x20;

    String_default_ctor(m0);
    String_default_ctor(m1);
    String_default_ctor(m2);

    String_cstr_ctor(tmp, RH_lit0, false);
    String_assign(m0, tmp);
    String_dtor(tmp);

    String_cstr_ctor(tmp, RH_lit1, false);
    String_assign(m1, tmp);
    String_dtor(tmp);

    String_cstr_ctor(tmp, RH_lit2, false);
    String_assign(m2, tmp);
    String_dtor(tmp);

    return self;
}
