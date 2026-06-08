#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_AEPakFile_vtable;

extern "C" uint32_t _ZN9AEPakFile7ReleaseEv(AEPakFile *self);

extern "C" AEPakFile *_ZN9AEPakFileD2Ev(AEPakFile *self)
{
    P(self, 0) = (char *)g_AEPakFile_vtable + 8;
    _ZN9AEPakFile7ReleaseEv(self);
    return self;
}
