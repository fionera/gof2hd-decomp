#include "class.h"

// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
// Mangled names so the demangled symbol contains "~PlayerFixedObject".
extern "C" void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self); // complete object dtor
extern "C" void operator_delete(void *p);

extern "C" void _ZN17PlayerFixedObjectD0Ev(PlayerFixedObject *self)
{
    return operator_delete(_ZN17PlayerFixedObjectD1Ev(self));
}
