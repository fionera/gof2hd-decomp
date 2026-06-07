#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_AENormalFile_vtable;

extern "C" uint32_t _ZN12AENormalFile7ReleaseEv(AENormalFile *self);

extern "C" AENormalFile *_ZN12AENormalFileD2Ev(AENormalFile *self)
{
    P(self, 0) = (char *)g_AENormalFile_vtable + 8;
    _ZN12AENormalFile7ReleaseEv(self);
    return self;
}
