#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_MTitle_vtable;

extern "C" void _ZN6MTitle9OnReleaseEv(MTitle *self);

extern "C" MTitle *_ZN6MTitleD2Ev(MTitle *self)
{
    self->f_0 = (char *)g_MTitle_vtable + 8;
    _ZN6MTitle9OnReleaseEv(self);
    return self;
}
