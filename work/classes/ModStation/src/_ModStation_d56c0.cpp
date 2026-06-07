#include "class.h"

// ModStation::~ModStation() base/inner destructor at 0xd56c0.
// PIC global: the ModStation vtable base (hidden -> direct pc-relative single deref).
__attribute__((visibility("hidden"))) extern void *ModStation_vtable;

extern "C" void ModStation_OnRelease(ModStation *self);
extern "C" void ModStation_String_dtor(void *s);

// Installs the vtable (+8), runs OnRelease(this), destroys the String member at
// +0x38, and returns `this` in r0 (consumed by the deleting dtor's tail-call).
extern "C" ModStation *ModStation_dtor_inner(ModStation *self)
{
    *(void **)self = (char *)ModStation_vtable + 8;
    ModStation_OnRelease(self);
    ModStation_String_dtor((char *)self + 0x38);
    return self;
}
