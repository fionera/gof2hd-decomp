#include "class.h"

// Mission::~Mission() (the inner/base dtor at 0x15ba54) returns `this` in r0.
extern "C" Mission *Mission_dtor_inner(Mission *self);
// Deallocation/base helper tail-called at the end (0x1ab098).
extern "C" void Mission_dtor_finish(Mission *self);

// Deleting/complete destructor: runs the inner dtor (which returns `this`), then
// tail-calls the finisher with that pointer — so `this` need not be saved.
extern "C" void Mission_dtor(Mission *self) {
    Mission_dtor_finish(Mission_dtor_inner(self));
}
