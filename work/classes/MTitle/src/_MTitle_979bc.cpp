#include "class.h"

extern "C" MTitle *_ZN6MTitleD2Ev(MTitle *self);
extern "C" void MTitle_deleteTail(MTitle *self);

extern "C" void _ZN6MTitleD0Ev(MTitle *self)
{
    MTitle_deleteTail(_ZN6MTitleD2Ev(self));
}
