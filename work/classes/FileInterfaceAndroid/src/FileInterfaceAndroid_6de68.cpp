#include "class.h"

struct sFILE;
// Vtable group base + an instance-count cell, both PC-relative globals.
extern void *gFIAVtable_file __attribute__((visibility("hidden")));
extern int *gFIAInstCount __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid(__sFILE*, bool) — wraps a stdio FILE*.
// NOTE: near-match. Identical instruction multiset; clang -Oz schedules the
// instance-count pointer deref and colors its temp register (r0 vs r1) differently
// than the target. Scheduling/coloring-only difference.
extern "C" void FileInterfaceAndroid_ctor_file(FileInterfaceAndroid *self, sFILE *f, bool append)
{
    int *cnt = gFIAInstCount;
    self->f_8 = f;
    self->f_c = 0;
    self->f_10 = 0;
    self->f_0 = (char *)gFIAVtable_file + 8;
    self->f_14 = append;
    *cnt = *cnt + 1;
}
