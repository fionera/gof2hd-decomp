#include "Wanted.h"


extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" Wanted *String_default_ctor(Wanted *self);
extern "C" void String_assign(Wanted *self, const String12 *rhs);
extern "C" void operator_delete_array(void *p);
extern "C" Wanted *Wanted_base_dtor(Wanted *self);

// ---- isTerminated_12298a.cpp ----
extern "C" uint8_t Wanted_isTerminated(Wanted *self) {
    return F<uint8_t>(self, 0x50);
}

// ---- setTerminated_12296c.cpp ----
extern "C" void Wanted_setTerminated(Wanted *self, bool v) {
    F<uint8_t>(self, 0x50) = v;
}

// ---- isActive_122984.cpp ----
extern "C" uint8_t Wanted_isActive(Wanted *self) {
    return F<uint8_t>(self, 0x51);
}

// ---- setActive_122972.cpp ----
extern "C" void Wanted_setActive(Wanted *self, bool v) {
    F<uint8_t>(self, 0x51) = v;
}

// ---- getName_122924.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the name String (stored at offset 0x0) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Wanted_getName(Wanted *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x00, false);
    return r;
}

// ---- Wanted_122890.cpp ----
// String::String(String* this) default ctor -> 0x6efbc (returns this in r0)
// String::operator=(String* this, const String* rhs) -> 0x6f2b0

// The String argument has a non-trivial copy ctor/dtor in the real engine, so the
// C++ ABI passes it by invisible reference -> it arrives as a pointer in r2.
extern "C" Wanted *
Wanted_ctor(Wanted *self, int p1, const String12 &p2, int p3, int p4, bool p5,
            int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14)
{
    Wanted *r = String_default_ctor(self);
    r->f_c = p1;
    String_assign(r, &p2);
    char *b = (char *)self;
    *(int *)(b + 0x10) = p3;
    *(int *)(b + 0x14) = p4;
    *(uint8_t *)(b + 0x18) = p5;
    *(uint16_t *)(b + 0x50) = 0;
    *(int *)(b + 0x44) = -1;
    *(int *)(b + 0x48) = -1;
    *(int *)(b + 0x4c) = -1;
    *(int *)(b + 0x1c) = p6;
    *(int *)(b + 0x20) = p7;
    *(int *)(b + 0x24) = p8;
    *(int *)(b + 0x28) = p9;
    *(int *)(b + 0x2c) = p10;
    *(int *)(b + 0x30) = p11;
    *(int *)(b + 0x34) = p12;
    *(int *)(b + 0x38) = p13;
    *(int *)(b + 0x3c) = p14;
    return self;
}

// ---- _Wanted_122904.cpp ----
// operator delete[](void*) -> 0x6ebfc
// Base subobject destructor (String::~String at offset 0), returns the object pointer.

// Frees the buffer at +0x40 (array delete), clears it, then tail-calls the base dtor.
extern "C" Wanted *Wanted_dtor(Wanted *self)
{
    void *p = self->f_40;
    if (p != 0) {
        operator_delete_array(p);
    }
    self->f_40 = 0;
    return Wanted_base_dtor(self);
}
