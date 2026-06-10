#include "gof2/Wanted.h"
#include "gof2/String.h"


using AbyssEngine::String12;

extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" Wanted *String_default_ctor(Wanted *self);
extern "C" void operator_delete_array(void *p);
extern "C" Wanted *Wanted_base_dtor(Wanted *self);

// ---- isTerminated_12298a.cpp ----
uint8_t Wanted::isTerminated() {
    Wanted *self = this;
    return self->field_0x50;
}

// ---- setTerminated_12296c.cpp ----
void Wanted::setTerminated(bool v) {
    Wanted *self = this;
    self->field_0x50 = v;
}

// ---- isActive_122984.cpp ----
uint8_t Wanted::isActive() {
    Wanted *self = this;
    return self->field_0x51;
}

// ---- setActive_122972.cpp ----
void Wanted::setActive(bool v) {
    Wanted *self = this;
    self->field_0x51 = v;
}

// ---- getName_122924.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the name String (stored at offset 0x0) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

RetStr Wanted::getName() {
    Wanted *self = this;
    RetStr r;
    String_copy_ctor(&r, &self->field_0x0, false);
    return r;
}

// ---- Wanted_122890.cpp ----
// String::String(String* this) default ctor -> 0x6efbc (returns this in r0)
// String::operator=(String* this, const String* rhs) -> 0x6f2b0

// The String argument has a non-trivial copy ctor/dtor in the real engine, so the
// C++ ABI passes it by invisible reference -> it arrives as a pointer in r2.
Wanted * Wanted::ctor(int p1, const String12 &p2, int p3, int p4, bool p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14) {
    Wanted *self = this;
    Wanted *r = String_default_ctor(self);
    r->field_0xc = p1;
    ((String *)(r))->assign(&p2);
    self->field_0x10 = p3;
    self->field_0x14 = p4;
    self->field_0x18 = p5;
    self->field_0x50 = 0;
    self->field_0x51 = 0;
    self->field_0x44 = -1;
    self->field_0x48 = -1;
    self->field_0x4c = -1;
    self->field_0x1c = p6;
    self->field_0x20 = p7;
    self->field_0x24 = p8;
    self->field_0x28 = p9;
    self->field_0x2c = p10;
    self->field_0x30 = p11;
    self->field_0x34 = p12;
    self->field_0x38 = p13;
    self->field_0x3c = p14;
    return self;
}

// ---- _Wanted_122904.cpp ----
// operator delete[](void*) -> 0x6ebfc
// Base subobject destructor (String::~String at offset 0), returns the object pointer.

// Frees the buffer at +0x40 (array delete), clears it, then tail-calls the base dtor.
Wanted * Wanted::dtor() {
    Wanted *self = this;
    void *p = self->field_0x40;
    if (p != 0) {
        operator_delete_array(p);
    }
    self->field_0x40 = 0;
    return Wanted_base_dtor(self);
}
