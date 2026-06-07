#include "class.h"

// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
extern "C" void *_ZN14PlayerGasCloudD1Ev(void *self); // complete object dtor
extern "C" void operator_delete(void *p);

extern "C" void _ZN14PlayerGasCloudD0Ev(void *self)
{
    return operator_delete(_ZN14PlayerGasCloudD1Ev(self));
}
