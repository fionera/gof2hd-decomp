#include "class.h"

// PIC globals (hidden -> single pc-relative deref): the Mission vtable base.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;
// String::String(String* this) default ctor -> 0x6efbc.
extern "C" void String_default_ctor(void *s);
// String::String(String* out, const char* cstr, bool) -> 0x6ee18.
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
// String::operator=(String* this, const String* rhs) -> 0x6f2b0.
extern "C" void String_assign(void *self, void *rhs);
// String::~String(String* this) -> 0x6ee30.
extern "C" void String_dtor(void *s);

// Mission::Mission() — default constructor. Installs the vtable, default-constructs
// the four String members, sets the name from a literal, and zero-inits the rest.
// The on-stack String temp triggers the -Oz stack-protector canary.
extern "C" Mission *Mission_ctor_default(Mission *self) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x10, tmp);
    String_dtor(tmp);
    self->f_64 = 0;
    self->f_74 = 0;
    self->f_8 = 0;
    self->f_c = -1;
    self->f_28 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
    self->f_58 = 0;
    self->f_5c = 0;
    self->f_70 = 0;
    self->f_4 = 0;
    self->f_38 = 0;
    self->f_3c = 0;
    return self;
}
