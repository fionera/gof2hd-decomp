#include "class.h"

// operator delete[](void*) -> 0x6ebfc
extern "C" void operator_delete_array(void *p);
// Base subobject destructor (String::~String at offset 0), returns the object pointer.
extern "C" Wanted *Wanted_base_dtor(Wanted *self);

// Frees the buffer at +0x40 (array delete), clears it, then tail-calls the base dtor.
extern "C" Wanted *Wanted_dtor(Wanted *self)
{
    void *p = F<void *>(self, 0x40);
    if (p != 0) {
        operator_delete_array(p);
    }
    F<void *>(self, 0x40) = 0;
    return Wanted_base_dtor(self);
}
