#include "class.h"

struct zip_file;
extern void *gFIAVtable_zip __attribute__((visibility("hidden")));
extern int *gFIAInstCount_zip __attribute__((visibility("hidden")));
extern "C" bool FileInterfaceAndroid_Seek(FileInterfaceAndroid *self, unsigned int off);

// FileInterfaceAndroid::FileInterfaceAndroid(zip_file*, bool, int, int, int) — wraps a libzip
// handle, bumps the instance count, then Seeks to the start offset. Returns `this`.
// NOTE: near-match (29/29 instr multiset identical; 2 instrs one scheduling slot off). clang
// -Oz orders the early `strd` and the `add.w r1,r2,#8` vptr finalize slightly differently from
// the target. Scheduling-only.
extern "C" FileInterfaceAndroid *
FileInterfaceAndroid_ctor_zip(FileInterfaceAndroid *self, zip_file *zf, bool append,
                              int start, int p4, int p5)
{
    int *cnt = gFIAInstCount_zip;
    char *vtbase = (char *)gFIAVtable_zip;   // *gVtable loaded; +8 applied at the store
    F<int>(self, 0x08) = 0;
    F<zip_file *>(self, 0x0c) = zf;
    F<int>(self, 0x10) = 0;
    u8(self, 0x14) = 0;
    F<char *>(self, 0x00) = vtbase + 8;
    *cnt = *cnt + 1;
    F<int>(self, 0x1c) = 0;
    F<int>(self, 0x28) = 0;
    FileInterfaceAndroid_Seek(self, start);
    u8(self, 0x24) = append;
    return self;
}
