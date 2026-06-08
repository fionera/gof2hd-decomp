#include "class.h"

// Complete-object destructor (D2). Re-establishes the vptr, closes any open handle, then either
// decrements the live-instance count or, when it is already zero, clears the alive byte at +0x4.
// Returns `this` (ARM EABI D1/D2 convention -> mov r0,r4; pop).
extern void *gFIAVtable_dtor __attribute__((visibility("hidden")));
extern int *gFIAInstCount_dtor __attribute__((visibility("hidden")));
extern "C" void FileInterfaceAndroid_Close(FileInterfaceAndroid *self);

FileInterfaceAndroid::~FileInterfaceAndroid()
{
    this->f_0 = (char *)gFIAVtable_dtor + 8;
    FileInterfaceAndroid_Close(this);
    int *cnt = gFIAInstCount_dtor;
    if (*cnt != 0)
        *cnt = *cnt - 1;
    else
        this->f_4 = 0;
}
