#include "class.h"

// Deleting destructor (D0): run the complete dtor, then tail-call operator delete.
extern "C" void *_ZN8KIPlayerD1Ev(KIPlayer *self); // complete object dtor
extern "C" void operator_delete(void *p);

extern "C" void _ZN8KIPlayerD0Ev(KIPlayer *self)
{
    return operator_delete(_ZN8KIPlayerD1Ev(self));
}
