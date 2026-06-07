#include "class.h"

// PIC globals: the Mission vtable base and the String::~String function pointer.
// Hidden visibility -> direct pc-relative single-deref (no GOT indirection),
// matching the target's `ldr; add pc; ldr` access.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern void (*String_dtor_fn)(void *s);

// Inner destructor: installs the vtable, then destroys the four String members
// (at 0x4c, 0x40, 0x1c, 0x10 — high to low), and returns `this`.
extern "C" Mission *Mission_dtor_inner(Mission *self) {
    void (*dtor)(void *) = String_dtor_fn;
    *(void **)self = (char *)Mission_vtable + 8;
    dtor((char *)self + 0x4c);
    dtor((char *)self + 0x40);
    dtor((char *)self + 0x1c);
    dtor((char *)self + 0x10);
    return self;
}
