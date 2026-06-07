#include "class.h"

// Base destructor (returns `this`), reached through its veneer at 0x74ee4.
extern "C" ModStation *ModStation_dtor_inner(ModStation *self);
// operator delete veneer tail-called at the end (0x1ab098).
extern "C" void ModStation_dtor_finish(ModStation *self);

// Deleting destructor at 0xd56f8: runs the base dtor (which returns `this`), then
// tail-calls the finisher with that pointer.
extern "C" void ModStation_dtor(ModStation *self)
{
    ModStation_dtor_finish(ModStation_dtor_inner(self));
}
