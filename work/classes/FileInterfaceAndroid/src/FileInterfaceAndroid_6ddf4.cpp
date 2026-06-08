#include "class.h"

// Vtable group base (PC-relative global); the installed vptr is *gVtable + 8.
extern void *gFIAVtable __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid() — default ctor.
// NOTE: near-match. Same instruction multiset; clang -Oz finalizes the vptr value
// (deref + `adds #8`) in a slightly different slot relative to the zero-stores than
// the target's interleaving. Scheduling-only difference.
extern "C" void FileInterfaceAndroid_ctor_default(FileInterfaceAndroid *self)
{
    self->f_4 = 1;
    char *vt = (char *)gFIAVtable + 8;
    self->f_30 = 0;
    self->f_34 = 0;
    self->f_24 = 0;
    self->f_28 = 0;
    self->f_0 = vt;
}
