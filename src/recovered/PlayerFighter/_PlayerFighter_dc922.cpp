#include "class.h"

// PlayerFighter deleting destructor (D0): run the complete-object dtor, then tail-call delete.
extern "C" void *_ZN13PlayerFighterD1Ev(PlayerFighter *self);
extern "C" void operator_delete(void *p);

extern "C" void _ZN13PlayerFighterD0Ev(PlayerFighter *self)
{
    return operator_delete(_ZN13PlayerFighterD1Ev(self));
}
