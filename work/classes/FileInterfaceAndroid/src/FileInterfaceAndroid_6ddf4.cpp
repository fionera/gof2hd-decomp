#include "class.h"

// Vtable group base (PC-relative global); the installed vptr is *gVtable + 8.
extern void *gFIAVtable __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid() — default ctor.
// NOTE: near-match. Same instruction multiset; clang -Oz finalizes the vptr value
// (deref + `adds #8`) in a slightly different slot relative to the zero-stores than
// the target's interleaving. Scheduling-only difference.
extern "C" void FileInterfaceAndroid_ctor_default(FileInterfaceAndroid *self)
{
    u8(self, 0x04) = 1;
    char *vt = (char *)gFIAVtable + 8;
    F<void *>(self, 0x30) = 0;
    F<void *>(self, 0x34) = 0;
    u8(self, 0x24) = 0;
    F<int>(self, 0x28) = 0;
    F<char *>(self, 0x00) = vt;
}
