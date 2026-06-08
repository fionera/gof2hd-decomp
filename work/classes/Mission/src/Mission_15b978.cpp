#include "class.h"

// PIC global (hidden -> single pc-relative deref): the Mission vtable base.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;
extern "C" void String_default_ctor(void *s);
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
extern "C" void String_assign(void *self, void *rhs);
extern "C" void String_dtor(void *s);

// Mission::Mission(int) — like the default ctor but stores the given id at +0xc.
extern "C" Mission *Mission_ctor_int(Mission *self, int id) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x10, tmp);
    String_dtor(tmp);
    F<int>(self, 0x64) = 0;
    F<uint8_t>(self, 0x74) = 0;
    F<int>(self, 0x8) = 0;
    F<int>(self, 0xc) = id;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = 0;
    F<int>(self, 0x30) = 0;
    F<int>(self, 0x58) = 0;
    F<uint8_t>(self, 0x5c) = 0;
    F<int>(self, 0x70) = 0;
    F<uint16_t>(self, 0x4) = 0;
    F<int>(self, 0x38) = 0;
    F<int>(self, 0x3c) = 0;
    return self;
}
